#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../generated/number_mock.grpc.pb.h"
#include "../streaming-client/streaming-client.h"

using namespace testing;

// check out here for more info: https://grpc.github.io/grpc/cpp/md_doc_unit_testing.html

TEST(StreamingClientTests, OnNextShouldReturnFizzBuzzCorrespondingValue)
{
	// MockNumberServiceStub is auto-generated with `generate_mock_code=true` when protoc is invoked (but you can define it by hand)
	auto serviceMock = std::make_unique<MockNumberServiceStub>();

	// fake responses that our mocked services will return to our client
	NumberResponse fakeResponse1;
	fakeResponse1.set_value(1);
	NumberResponse fakeResponse2;
	fakeResponse2.set_value(2);
	NumberResponse fakeResponse3;
	fakeResponse3.set_value(3);

	// since we have Next(ClientContext*, const NumberRequest&, NumberResponse*)
	// we can set the third argument to the response we want our service to reply back
	EXPECT_CALL(*serviceMock, Next(_, _, _))
		.WillOnce(DoAll(SetArgPointee<2>(fakeResponse1), Return(grpc::Status::OK)))
		.WillOnce(DoAll(SetArgPointee<2>(fakeResponse2), Return(grpc::Status::OK)))
		.WillOnce(DoAll(SetArgPointee<2>(fakeResponse3), Return(grpc::Status::OK)));

	const StreamingClient client(std::move(serviceMock));
	EXPECT_THAT(client.Next(), Eq("1"));
	EXPECT_THAT(client.Next(), Eq("2"));
	EXPECT_THAT(client.Next(), Eq("Fizz"));
}

// taken from here: https://github.com/grpc/grpc/blob/master/include/grpcpp/test/mock_stream.h
template <class R>
class MockClientReader : public grpc::ClientReaderInterface<R>
{
public:
	MockClientReader() = default;

	/// ClientStreamingInterface
	MOCK_METHOD0_T(Finish, grpc::Status());

	/// ReaderInterface
	MOCK_METHOD1_T(NextMessageSize, bool(uint32_t*));
	MOCK_METHOD1_T(Read, bool(R*));

	/// ClientReaderInterface
	MOCK_METHOD0_T(WaitForInitialMetadata, void());
};

TEST(StreamingClientTests, OnRangeShouldDoubleTheReceivedValue)
{
	// IMPORTANT PATTERN: this is a raw pointer because `NumberService::StubInterface` will wrap it into a unique_ptr
	//					  StubInterface uses NVI idiom (non-virtual interface)
	const auto clientReaderMock = new MockClientReader<RangeResponse>();
	// for better clarity, use `owner<T*>` from GSL (or some other type that makes clear you don't have to destroy this pointer)
	
	// as before, these are some fakes responses our service will stream back to the client
	RangeResponse fakeResponse1;
	fakeResponse1.set_value(1);
	RangeResponse fakeResponse2;
	fakeResponse2.set_value(2);

	// Read calls on MockClientReader (the last one will terminate the streaming by returning `false`)
	EXPECT_CALL(*clientReaderMock, Read(_))
		.WillOnce(DoAll(SetArgPointee<0>(fakeResponse1), Return(true)))
		.WillOnce(DoAll(SetArgPointee<0>(fakeResponse2), Return(true)))
		.WillOnce(DoAll(SetArgPointee<0>(RangeResponse{}), Return(false)));

	// we also expect Finish will be called from the client
	EXPECT_CALL(*clientReaderMock, Finish()).
		WillOnce(Return(grpc::Status::OK));

	// finally, we prepare the service mock
	auto serviceMock = std::make_unique<MockNumberServiceStub>();

	// `RangeRaw` is the proper name since it returns the "raw" pointer to `ClientReaderInterface`
	// here we also save the request our client makes and passes to the service to check it's done properly
	RangeRequest actualRequest;
	EXPECT_CALL(*serviceMock, RangeRaw(_, _))
		.WillOnce(DoAll(SaveArg<1>(&actualRequest), Return(clientReaderMock)));

	const StreamingClient client(std::move(serviceMock));
	std::vector<uint64_t> actuals;
	client.Range(0, 2, [&](uint64_t value) {
		actuals.push_back(value);
	});

	EXPECT_THAT(actualRequest.min(), Eq(0));
	EXPECT_THAT(actualRequest.max(), Eq(2));
	EXPECT_THAT(actuals, ElementsAre(2, 4));
}