#include "streaming-client.h"
#include <chrono>

using namespace std::chrono_literals;

StreamingClient::StreamingClient(std::unique_ptr<NumberService::StubInterface> stub)
	: m_stub(std::move(stub))
{
}

static std::string fizzBuzz(uint64_t n)
{
	if (n % 15 == 0)
		return "FizzBuzz";
	if (n % 3 == 0)
		return "Fizz";
    if (n % 5 == 0)
		return "Buzz";
	return std::to_string(n);
}

std::string StreamingClient::Next() const
{
	grpc::ClientContext ctx;
	NumberResponse response;
	if (const auto status = m_stub->Next(&ctx, {}, &response); !status.ok())
	{
		throw std::runtime_error("Error communicating to the service...");
	}
	return fizzBuzz(response.value());
}

void StreamingClient::Range(uint64_t min, uint64_t max, std::function<void(uint64_t)> onValue) const
{
	RangeRequest request;
	request.set_min(min);
	request.set_max(max);
	grpc::ClientContext ctx;
	auto reader = m_stub->Range(&ctx, request);
	RangeResponse response;
	while (reader->Read(&response))
	{
		onValue(response.value() * 2);
	}
	if (const auto status = reader->Finish(); !status.ok())
	{
		throw std::runtime_error(std::format("Sum got an error from the service: {}", status.error_message()));
	}
}

uint64_t StreamingClient::Sum(std::span<uint64_t> values) const
{
	grpc::ClientContext ctx;
	SumResponse response;
	SumRequest request;
	auto writer = m_stub->Sum(&ctx, &response);
	for (auto i : values)
	{
		request.set_value(i);
		writer->Write(request);		
	}
	writer->WritesDone();
	if (const auto status = writer->Finish(); !status.ok())
	{
		throw std::runtime_error(std::format("Sum got an error from the service: {}", status.error_message()));
	}
	return response.value();
}
