#include <grpcpp/impl/codegen/server_context.h>
#include "../generated/snowflake.grpc.pb.h"
#include <grpc++/server_builder.h>
#include <queue>
#include "snowflake.h"

using namespace grpc;

// naively synchronized queue for our purposes
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

/* This is a very simple example of bidirectional streaming.
   A snowflake (https://en.wikipedia.org/wiki/Snowflake_ID) is a sequential 64bit unique identifier (invented by Twitter and used also by Discord and Instagram).
   Here, the service is able to generate subsequent snowflakes by assigning the client a free "workerId" (ranging from 0 to 31).
   A client does the request and then ask for ids. The service replies with snowflakes until the streaming session is up.
   When done, the workerId can be reused.
   Multiple clients can simultaneously open a streaming session, thus the access to the repository of free workerIds is (naively) synchronized.
   Since, in the original implementation, snowflake supports up to 32 workerIds, the 33rd streaming request is refused.
*/
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
	//							v-- in the original implementation, this "dataCenterId" (0-31) is just a configurable setting	
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