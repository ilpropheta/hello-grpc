#include <grpcpp/create_channel.h>
#include "../generated/hello.grpc.pb.h"

int main()
{
	// represents a connection to a certain endpoint (where many services can be available)
	auto channel = CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());

	// the specific service is here: "one stub = one client" of a certain service
	// indeed, the same channel might be shared for interacting with multiple services over the same connection
	auto stub = HelloService::NewStub(std::move(channel));

	grpc::ClientContext ctx;	
	HelloRequest request;
	request.set_name("Marco");
	HelloResponse response;
	if (const auto status = stub->SayHello(&ctx, request, &response); status.ok())
	{
		std::cout << response.message() << "\n";
	}
	else
	{
		std::cout << "error communicating to the server: " << status.error_code();
		return status.error_code();
	}
}
