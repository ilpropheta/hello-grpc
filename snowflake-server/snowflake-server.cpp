#include <grpcpp/impl/codegen/server_context.h>
#include "../generated/snowflake.grpc.pb.h"
#include <grpc++/server_builder.h>
#include <queue>
#include "snowflake.h"

using namespace grpc;

class id_repo
{
public:
	explicit id_repo(std::queue<int64_t> s)
		: m_data(std::move(s))
	{
		
	}

	void enqueue(size_t id)
	{
		return OnLockedResource([=](auto& st) {
			return st.push(id);
		});
	}

	std::optional<int64_t> dequeue()
	{
		return OnLockedResource([](auto& st) -> std::optional<int64_t> {
			if (!st.empty())
			{
				auto elem = st.front();
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
	std::queue<int64_t> m_data;
};

using snowflake_t = snowflake<>;

static id_repo CreateSnowflakeWorkerIds()
{
	std::queue<int64_t> st;
	for (auto i = 0u; i <= snowflake_t::MAX_WORKER_ID; ++i)
		st.push(i);
	return id_repo{std::move(st)};
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
		if (const auto id = m_workerIds.dequeue(); id)
		{
			snowflake_t generator(*id, m_dataCenterId);
			NextIdRequest request;
			while (stream->Read(&request))
			{
				NextIdResponse response;
				response.set_value(generator.next_id());
				stream->Write(response);
			}
			std::cout << "WorkerId " << *id << " has finished..." << "\n";
			m_workerIds.enqueue(*id);
			return Status::OK;
		}
		return Status{ RESOURCE_EXHAUSTED, "Too many requests..."};
	}
private:
	id_repo m_workerIds = CreateSnowflakeWorkerIds();
	size_t m_dataCenterId;
};

int main()
{
	SnowflakeServerImpl service{1};
	ServerBuilder builder;
	builder.AddListeningPort("localhost:50051", InsecureServerCredentials());
	builder.RegisterService(&service);
	auto server = builder.BuildAndStart();
	std::cout << "The service is listening! Press Enter to shutdown\n";
	std::cin.get();
	server->Shutdown();
	server->Wait();
}