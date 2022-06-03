#define _WINSOCKAPI_
#include <csignal>
#include <so_5/all.hpp>
#include <Windows.h>
#include <utility>
#include <atomic>
#include <ranges>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/udp_sink.h>
#include "../generated/broker.grpc.pb.h"
#include "../generated/broker.pb.h"
#include <grpc++/server_builder.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using namespace grpc;

template<typename T>
struct std::formatter<google::protobuf::RepeatedPtrField<T>>
{
	constexpr auto parse(std::format_parse_context& ctx)
	{
		return ctx.begin();
	}

	auto format(const google::protobuf::RepeatedPtrField<T>& repeated, std::format_context& ctx) const
	{
		if (repeated.empty())
			return ctx.out();
		for (const auto& field : std::views::take(repeated, repeated.size() - 1))
		{
			std::format_to(ctx.out(), "{}, ", field);
		}
		return std::format_to(ctx.out(), "{}", *prev(repeated.end()));
	}
};

class read_agent : public so_5::agent_t
{
	struct connection_check_timeout : so_5::signal_t {};
public:
	read_agent(context_t c, ServerContext* context, ServerWriter<ReceiveResponse>* writer, std::vector<so_5::mbox_t> channels)
		: agent_t(std::move(c)), m_context(context), m_writer(writer), m_channels(std::move(channels))
	{
	}

	Status Read() const
	{
		m_over.wait(m_over.load());
		spdlog::debug("A client worker has been released");
		return Status::OK;
	}

private:
	void so_define_agent() override
	{
		for (const auto& channel : m_channels)
		{
			so_subscribe(channel).event([chanName = channel->query_name(), this](so_5::mhood_t<std::string> data) {
				spdlog::debug("A client worker got a message '{}' on channel '{}' - thread {}", *data, chanName, GetCurrentThreadId());
				ReceiveResponse response;
				response.mutable_message()->set_topic(chanName);
				response.mutable_message()->set_content(*data);
				const auto writeSuccessful = m_writer->Write(response);
				spdlog::debug("A client worker sent message to subscriber. Success={}", writeSuccessful);
				if (!writeSuccessful)
				{
					DeactivateThisAgent();
				}
			});
		}

		so_subscribe_self().event([this](so_5::mhood_t<connection_check_timeout>) {
			if (m_context->IsCancelled())
			{
				spdlog::debug("A client worker detected a client disconnection...will detach");
				DeactivateThisAgent();
			}
		});
	}

	void DeactivateThisAgent()
	{
		m_connectionTimer = {};
		m_over = true;
		so_deactivate_agent();
		so_deregister_agent_coop_normally();
		m_over.notify_all();
		spdlog::debug("A client worker detected a client disconnection...detaching procedures done");
	}

	void so_evt_start() override
	{
		spdlog::debug("Worker on thread {} started", GetCurrentThreadId());
		m_connectionTimer = so_5::send_periodic<connection_check_timeout>(so_direct_mbox(), std::chrono::seconds(5), std::chrono::seconds(5));
	}

	void so_evt_finish() override
	{
		spdlog::debug("Worker on thread {} finished", GetCurrentThreadId());
		m_over = true;
		m_over.notify_all();
	}

	ServerContext* m_context;
	ServerWriter<ReceiveResponse>* m_writer;
	std::vector<so_5::mbox_t> m_channels;
	std::atomic<bool> m_over = false;
	so_5::timer_id_t m_connectionTimer;
};

class ServiceImpl : public MessageBroker::Service, public so_5::agent_t
{
public:
	ServiceImpl(context_t c)
		: agent_t(std::move(c))
	{
		constexpr auto threadPoolSize = 5;
		spdlog::debug("Starting service with thread pool size={}", threadPoolSize);
		m_binder = so_5::disp::thread_pool::make_dispatcher(so_environment(), threadPoolSize).binder();
		m_parentCoop = so_environment().register_coop(so_environment().make_coop(m_binder));
	}

	Status Send([[maybe_unused]]ServerContext* context, const SendRequest* request, [[maybe_unused]] SendResponse* response) override
	{		
		for (const auto& message : request->messages())
		{
			send<std::string>(so_environment().create_mbox(message.topic()), message.content());
			spdlog::debug("A client dropped a message '{}' to topic '{}'", message.content(), message.topic());
		}
		return Status::OK;
	}

	Status Receive(ServerContext* context, const ReceiveRequest* request, ServerWriter<ReceiveResponse>* writer) override
	{
		spdlog::debug("A client subscribed to topics '{}'", request->topics());
		auto coop = so_environment().make_coop(m_parentCoop, m_binder);
		const auto agent = coop->make_agent<read_agent>(context, writer, GetChannelsFrom(*request));
		auto regCoop = so_environment().register_coop(std::move(coop));
		return agent->Read();
	}
private:
	std::vector<so_5::mbox_t> GetChannelsFrom(const ReceiveRequest& request)
	{
		std::vector<so_5::mbox_t> channels(request.topics().size());
		std::transform(begin(request.topics()), end(request.topics()), begin(channels), [&](const auto& name) {
			return so_environment().create_mbox(name);
		});
		return channels;
	}

	so_5::coop_handle_t m_parentCoop;
	so_5::disp_binder_shptr_t m_binder;
};

std::promise<void> stop;

static void TerminateThisProgram(int sig)
{
	if (sig == SIGTERM || sig == SIGINT)
	{
		spdlog::debug("got termination signal");
		stop.set_value();
	}
}

int main()
{
	try
	{
		spdlog::set_level(spdlog::level::debug);
		set_default_logger(spdlog::stdout_color_mt("Message-Broker"));

		std::signal(SIGINT, TerminateThisProgram);
		std::signal(SIGTERM, TerminateThisProgram);

		so_5::wrapped_env_t sobj;
		auto coop = sobj.environment().make_coop(so_5::disp::active_obj::make_dispatcher(sobj.environment()).binder());
		auto* agent = coop->make_agent<ServiceImpl>();
		sobj.environment().register_coop(std::move(coop));

		reflection::InitProtoReflectionServerBuilderPlugin();
		EnableDefaultHealthCheckService(true);
		const std::string server_address = "localhost:50051";
		ServerBuilder builder;
		builder.AddListeningPort(server_address, InsecureServerCredentials());
		builder.RegisterService(agent);
		auto server = builder.BuildAndStart();
		server->GetHealthCheckService()->SetServingStatus("MessageBroker", true);
		spdlog::info("Server is listening on {}", server_address);

		stop.get_future().wait();
		spdlog::debug("Program terminated");
		sobj.stop_then_join();
		spdlog::debug("Agents terminated");
		server->Shutdown();
		server->Wait();
		spdlog::debug("Server wait is over");
	}
	catch (const std::exception& ex)
	{
		spdlog::critical("Unrecoverable error: {}", ex.what());
		return 1;
	}
	spdlog::info("Exiting successfully");
}