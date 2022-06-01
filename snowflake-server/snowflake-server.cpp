#include <grpcpp/impl/codegen/server_context.h>
#include "../generated/snowflake.grpc.pb.h"
#include <grpc++/server_builder.h>
#include <stack>
#include "snowflake.h"

using namespace grpc;

class naive_stack
{
public:
	explicit naive_stack(std::stack<int64_t> s)
		: m_data(std::move(s))
	{
		
	}

	void push(size_t id)
	{
		return OnLockedResource([=](auto& st) {
			return st.push(id);
		});
	}

	std::optional<int64_t> try_pop()
	{
		return OnLockedResource([](auto& st) -> std::optional<int64_t> {
			if (!st.empty())
			{
				auto elem = st.top();
				st.pop();
				return elem;
			}
			return std::nullopt;
		});
	}
	
private:
	template<typename Action>
	auto OnLockedResource(Action action)
	{
		std::lock_guard l{ m_mutex };
		return action(m_data);
	}
	
	std::mutex m_mutex;
	std::stack<int64_t> m_data;
};

static naive_stack CreateIds()
{
	std::stack<int64_t> st;
	for (auto i = 0u; i <= snowflake<>::MAX_WORKER_ID; ++i)
		st.push(i);
	return naive_stack{std::move(st)};
}

class SnowflakeServerImpl final : public SnowflakeServer::Service
{
public:
	explicit SnowflakeServerImpl(size_t dataCenterId)
		: m_dataCenterId(dataCenterId)
	{
		
	}
	
	Status NextId(ServerContext* context, ServerReaderWriter<NextIdResponse, NextIdRequest>* stream) override
	{
		if (const auto id = m_ids.try_pop(); id)
		{
			snowflake generator(*id, m_dataCenterId);
			NextIdRequest request;
			while (stream->Read(&request))
			{
				NextIdResponse response;
				response.set_value(generator.next_id());
				stream->Write(response);
			}
			std::cout << "WorkerId " << *id << " has finished..." << "\n";
			m_ids.push(*id);
			return Status::OK;
		}
		return Status{ RESOURCE_EXHAUSTED, "Too many requests..."};
	}
private:
	naive_stack m_ids = CreateIds();
	size_t m_dataCenterId;
};

int main()
{
	EnableDefaultHealthCheckService(true);
	SnowflakeServerImpl service{1};
	ServerBuilder builder;
	builder.AddListeningPort("localhost:50051", InsecureServerCredentials());
	builder.RegisterService(&service);
	auto server = builder.BuildAndStart();
	server->GetHealthCheckService()->SetServingStatus("HelloService", true);
	std::cout << "The service is listening! Press Enter to shutdown\n";
	std::cin.get();
	server->Shutdown();
	server->Wait();
}