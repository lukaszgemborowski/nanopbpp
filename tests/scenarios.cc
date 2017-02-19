#include <gtest/gtest.h>
#include <vector>
#include <nanopbpp/meta_encoder.h>
#include <nanopbpp/message_encoder.h>
#include <nanopbpp/meta_decoder.h>
#include <nanopbpp/message_decoder.h>

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

TEST(scenarios, encode_and_decode_message_with_extensions)
{
	using namespace nanopbpp;

	std::array<uint8_t, 64> buffer = {0};
	IntegerContainer src_a = {0}, dst_a = {0};
	FloatContainer src_b = {0}, dst_b = {0};

	src_a.a = 1;
	src_a.has_b = true;
	src_a.b = 2;
	src_b.c = 3.4f;

	auto source = messages_metadata.create<Extendable>();
	source.extension_by_tag<field_a_tag>().attach_to_storage(src_a);
	source.extension_by_tag<field_b_tag>().attach_to_storage(src_b);

	ASSERT_TRUE(encode(buffer.begin(), buffer.end(), source));

	auto destination = messages_metadata.create<Extendable>();
	destination.extension_by_tag<field_a_tag>().attach_to_storage(dst_a);
	destination.extension_by_tag<field_b_tag>().attach_to_storage(dst_b);

	ASSERT_TRUE(decode(buffer.begin(), buffer.end(), destination));

	ASSERT_EQ(src_a.a, dst_a.a);
	ASSERT_EQ(src_a.has_b, dst_a.has_b);
	ASSERT_EQ(src_a.b, dst_a.b);
	ASSERT_EQ(src_b.c, dst_b.c);
}
