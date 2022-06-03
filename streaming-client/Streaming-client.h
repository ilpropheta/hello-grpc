#pragma once
#include "../generated/number.grpc.pb.h"
#include <span>

class StreamingClient
{
public:
	// you can initialize this also with `NumberService::NewStub(channel)`
	explicit StreamingClient(std::unique_ptr<NumberService::StubInterface> stub);

	[[nodiscard]] std::string Next() const;
	void Range(uint64_t min, uint64_t max, std::function<void(uint64_t)> onValue) const;
	unsigned long long Sum(std::span<uint64_t> values) const;
private:
	std::unique_ptr<NumberService::StubInterface> m_stub;
};