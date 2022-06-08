#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "../streaming-client/Streaming-client.h"

using namespace grpc;

// use this to play around with the MagicNumberService (streaming-service)
int main()
{
	const StreamingClient client(NumberService::NewStub(CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())));

	client.Range(0, 10, [](auto value) {
		// let's check what happens if the client is slow at reading back from the server...
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "client just got: " << value << "\n";
	});

	std::cin.get();
}