#include <gtest/gtest.h>
#include "test.pb.h"
#include <vector>
#include <nanopbpp/basic_decoder.h>
#include <pb_encode.h>

TEST(basic_decoder_tests, encode_decode_basic_buffer)
{
	std::vector<uint8_t> buffer(512);
	IntegerContainer source = { 0 };
	IntegerContainer destination = { 0 };

	source.a = 1;
	source.has_b = true;
	source.b = 2;

	auto ostream = pb_ostream_from_buffer(buffer.data(), buffer.size());
	ASSERT_TRUE(pb_encode(&ostream, IntegerContainer_fields, &source));

	auto decoder = nanopbpp::create_decoder(buffer.begin(), buffer.begin() + ostream.bytes_written);
	ASSERT_TRUE(decoder.decode(IntegerContainer_fields, &destination));

	ASSERT_EQ(source.a, destination.a);
	ASSERT_EQ(source.has_b, destination.has_b);
	ASSERT_EQ(source.b, destination.b);
}

