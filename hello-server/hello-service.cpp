#include <grpcpp/impl/codegen/server_context.h>
#include "../generated/hello.grpc.pb.h"
#include <grpc++/server_builder.h>
#include <format>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

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
	grpc::EnableDefaultHealthCheckService(true);
	HelloServiceImpl service;
	grpc::ServerBuilder builder;
	builder.AddListeningPort("localhost:50051", grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	auto server = builder.BuildAndStart();
	server->GetHealthCheckService()->SetServingStatus("HelloService", true);
	std::cout << "The service is listening! Press Enter to shutdown\n";
	std::cin.get();
	server->Shutdown();
	server->Wait();
}