#include <gtest/gtest.h>
#include <vector>
#include <nanopbpp/meta_encoder.h>
#include <nanopbpp/meta_decoder.h>

// metadata for all messages
#include "meta.h"

TEST(scenarios, encode_and_decode_simple_message_with_static_buffer)
{
	using namespace nanopbpp;

	IntegerContainer source = {0}, destination = {0};

	// prepare data to be encoded in protobuf, basically
	// you need to fill standard nanopb generated structure
	source.a = 5;
	source.has_b = true;
	source.b = 16;

	// prepare space for buffer
	std::vector<uint8_t> buffer(64);

	// encode message to buffer
	ASSERT_TRUE(encode(messages_metadata, buffer.begin(), buffer.end(), source));

	// decodemessage from buffer
	ASSERT_TRUE(decode(messages_metadata, buffer.begin(), buffer.end(), destination));

	ASSERT_EQ(source.a, destination.a);
	ASSERT_EQ(source.b, destination.b);
	ASSERT_EQ(source.has_b, destination.has_b);
}

TEST(scenarios, encode_and_decode_simple_message_with_array_buffer)
{
	using namespace nanopbpp;

	IntegerContainer source = {0}, destination = {0};

	// prepare data to be encoded in protobuf, basically
	// you need to fill standard nanopb generated structure
	source.a = 10;
	source.has_b = true;
	source.b = 20;

	std::array<uint8_t, 64> buffer = {0};

	// encode message to buffer
	ASSERT_TRUE(encode(messages_metadata, buffer.begin(), buffer.end(), source));

	// decodemessage from buffer
	ASSERT_TRUE(decode(messages_metadata, buffer.begin(), buffer.end(), destination));

	ASSERT_EQ(source.a, destination.a);
	ASSERT_EQ(source.b, destination.b);
	ASSERT_EQ(source.has_b, destination.has_b);
}

TEST(scenarios, encode_and_decode_simple_message_with_C_array)
{
	using namespace nanopbpp;

	IntegerContainer source = {0}, destination = {0};

	// prepare data to be encoded in protobuf, basically
	// you need to fill standard nanopb generated structure
	source.a = 10;
	source.has_b = true;
	source.b = 20;

	uint8_t buffer[64] = {0};

	// encode message to buffer
	ASSERT_TRUE(encode(messages_metadata, buffer, buffer + sizeof(buffer), source));

	// decodemessage from buffer
	ASSERT_TRUE(decode(messages_metadata, buffer, buffer + sizeof(buffer), destination));

	ASSERT_EQ(source.a, destination.a);
	ASSERT_EQ(source.b, destination.b);
	ASSERT_EQ(source.has_b, destination.has_b);
}
