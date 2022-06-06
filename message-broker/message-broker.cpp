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

// very simple way to format a protobuf sequence
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
		{
			return ctx.out();
		}
		for (const auto& field : std::views::take(repeated, static_cast<size_t>(repeated.size()) - 1))
		{
			std::format_to(ctx.out(), "{}, ", field);
		}
		return std::format_to(ctx.out(), "{}", *prev(repeated.end()));
	}
};

/* An agent for dispatching data to a certain client which has called "Receive" on some topics
  clearly, other options are possible, this is a just an example.
*/
class ReceiveAgent : public so_5::agent_t
{
	struct connection_check_timeout : so_5::signal_t {};
public:
	ReceiveAgent(context_t c, ServerContext* context, ServerWriter<ReceiveResponse>* writer, std::vector<so_5::mbox_t> channels)
		: agent_t(std::move(c)), m_context(context), m_writer(writer), m_channels(std::move(channels))
	{
	}

	Status Read() const
	{
		m_over.wait(m_over.load()); // simple way to just wait for completion
		spdlog::debug("A client worker has been released");
		return Status::OK;
	}

private:
	void so_define_agent() override
	{
		// let's subscribe to every topic (aka: 1 topic = 1 so_5::mbox_t)
		for (const auto& channel : m_channels)
		{
			so_subscribe(channel).event([chanName = channel->query_name(), this](so_5::mhood_t<std::string> data) {
				spdlog::debug("A client worker got a message '{}' on channel '{}' - thread {}", *data, chanName, GetCurrentThreadId());
				ReceiveResponse response;
				response.mutable_message()->set_topic(chanName);
				response.mutable_message()->set_content(*data);
				const auto writeSuccessful = m_writer->Write(response);
				spdlog::debug("A client worker sent message to subscriber. Success={}", writeSuccessful);
				// if we get a write error, the client has possibly gone.
				// Clearly, other options are possible (retry, just ignore this error, etc).
				if (!writeSuccessful)
				{
					DeactivateThisAgent();
				}
			});
		}

		// let's subscribe to connection_check_timeout in order to check if the client is still there
		so_subscribe_self().event([this](so_5::mhood_t<connection_check_timeout>) {
			if (m_context->IsCancelled())
			{
				spdlog::debug("A client worker detected a client disconnection...will detach");
				DeactivateThisAgent();
			}
		});
	}

	// some boilerplate needed to deactivate this agent, unsubscribe from mboxes, and free associated resources
	void DeactivateThisAgent()
	{
		m_connectionTimer = {}; // stop the timer (just to avoid any triggers in the middle of agent deactivation)
		so_deactivate_agent(); // unsubscribe + put in special "inactive" state
		so_deregister_agent_coop_normally(); // since we have "1 agent = 1 coop", we can directly drop the agent's cooperation to free the associated resources
		m_over = true; 
		m_over.notify_all(); // resume the waiting thread
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

/* An implementation of the MessageBroker service based on SObjectizer
*  Every "Receive" (aka: every client) is handled by a dedicated agent which subscribes to all the topics of interest of that particular request.
*/
class ServiceImpl : public MessageBroker::Service, public so_5::agent_t
{
public:
	ServiceImpl(context_t c)
		: agent_t(std::move(c))
	{
		constexpr auto threadPoolSize = 5;
		spdlog::debug("Starting service with thread pool size={}", threadPoolSize);
		// using a thread pool binder means that every agent's handler will be executed by a thread of the pool
		m_binder = so_5::disp::thread_pool::make_dispatcher(so_environment(), threadPoolSize).binder();
		// this "root" cooperation is useful if we want deregister every sub-cooperation at once (this feature is not implemented in this simple demo)
		m_rootCoop = so_environment().register_coop(so_environment().make_coop(m_binder));
	}

	// this is simply a so_5::send of all the messages
	// SObjectizer manages the named "topics" (aka: mailboxes) for us
	Status Send([[maybe_unused]]ServerContext* context, const SendRequest* request, [[maybe_unused]] SendResponse* response) override
	{		
		for (const auto& message : request->messages())
		{
			send<std::string>(so_environment().create_mbox(message.topic()), message.content());
			spdlog::debug("A client dropped a message '{}' to topic '{}'", message.content(), message.topic());
		}
		return Status::OK;
	}

	// every "Receive" is handled by creating a new "ReceiveAgent" that will reside in its own "cooperation".
	// The reason why every agent has its own coop_t is to ease deregistration.
	Status Receive(ServerContext* context, const ReceiveRequest* request, ServerWriter<ReceiveResponse>* writer) override
	{
		spdlog::debug("A client subscribed to topics '{}'", request->topics());
		// new agent's cooperation is a child of the root coop
		auto coop = so_environment().make_coop(m_rootCoop, m_binder);
		const auto agent = coop->make_agent<ReceiveAgent>(context, writer, GetChannelsFrom(*request));
		auto regCoop = so_environment().register_coop(std::move(coop));
		return agent->Read(); // blocks until the agent is done
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

	so_5::coop_handle_t m_rootCoop;
	so_5::disp_binder_shptr_t m_binder;
};

// termination is handled by subscribing to SIGINT and SIGTERM (e.g. CTRL+C)
// and by using a promise/future for event synchronization (even though, atomic_flag or condition_variable are possible options too).
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

		// this represents a SObjectizer environment that keeps every SObjectizer resource alive
		so_5::wrapped_env_t sobj;
		// every agent in SObjectizer resides in a cooperation
		auto coop = sobj.environment().make_coop(so_5::disp::active_obj::make_dispatcher(sobj.environment()).binder());
		auto* agent = coop->make_agent<ServiceImpl>();
		sobj.environment().register_coop(std::move(coop));

		// for our demo, we use the gRPC reflection plugin
		reflection::InitProtoReflectionServerBuilderPlugin();
		// on the same page, we enable the default health check service
		EnableDefaultHealthCheckService(true);
		const std::string server_address = "localhost:50051";
		ServerBuilder builder;
		builder.AddListeningPort(server_address, InsecureServerCredentials());
		builder.RegisterService(agent);
		auto server = builder.BuildAndStart();
		server->GetHealthCheckService()->SetServingStatus("MessageBroker", true);
		spdlog::info("Server is listening on {}", server_address);

		// let's wait for termination (CTRL+C or just SIGTERM/SIGINT)
		stop.get_future().wait();
		spdlog::debug("Program terminated");
		// first we turn off SObjectizer (in order to complete any pending requests)
		sobj.stop_then_join();
		spdlog::debug("Agents terminated");
		// finally we shutdown the server and wait for this to complete
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