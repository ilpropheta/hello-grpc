#include "../generated/number.grpc.pb.h"
#include <grpc++/server_builder.h>

using namespace grpc;

class MagicNumberService final : public NumberService::Service
{
public:
	Status Next([[maybe_unused]]ServerContext* context, [[maybe_unused]] const NumberRequest* request, NumberResponse* response) override
	{
		response->set_value(m_incremental++);
		return Status::OK;
	}

	Status Range([[maybe_unused]]ServerContext* context, const RangeRequest* request, ServerWriter<RangeResponse>* writer) override
	{
		RangeResponse response;
		for (auto i=request->min(); i<request->max(); ++i)
		{
			response.set_value(i);
			writer->Write(response);
		}
		return Status::OK;
	}

	Status Sum([[maybe_unused]]ServerContext* context, ServerReader<SumRequest>* reader, SumResponse* response) override
	{
		uint64_t sum = 0;
		SumRequest request;
		while (reader->Read(&request))
		{
			sum += request.value();
		}
		response->set_value(sum);
		return Status::OK;
	}
private:
	std::atomic<uint64_t> m_incremental = 0;
};


int main()
{
	MagicNumberService service;
	ServerBuilder builder;
	builder.AddListeningPort("localhost:50051", InsecureServerCredentials());
	builder.RegisterService(&service);
	auto server = builder.BuildAndStart();
	std::cout << "The service is listening! Press Enter to shutdown\n";
	std::cin.get();
	server->Shutdown();
	server->Wait();
}