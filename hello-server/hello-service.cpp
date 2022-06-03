#include <grpcpp/impl/codegen/server_context.h>
#include "../generated/hello.grpc.pb.h"
#include <grpc++/server_builder.h>
#include <format>

// our implementation of HelloService defined in hello.proto
class HelloServiceImpl final : public HelloService::Service
{
	grpc::Status SayHello(grpc::ServerContext*, const HelloRequest* request, HelloResponse* reply) override
	{
		std::cout << "got a request!\n";
		reply->set_message(std::format("Hello {}!", request->name()));
		return grpc::Status::OK;
	}
};

int main()
{
	HelloServiceImpl service;
	grpc::ServerBuilder builder;
	builder.AddListeningPort("localhost:50051", grpc::InsecureServerCredentials());
	builder.RegisterService(&service); // we can add more than one (different) service to the same server!
	auto server = builder.BuildAndStart();
	std::cout << "The service is listening! Press Enter to shutdown\n";
	std::cin.get();
	server->Shutdown();
	server->Wait();
}