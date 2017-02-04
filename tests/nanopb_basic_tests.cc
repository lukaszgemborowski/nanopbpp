#include <gtest/gtest.h>
#include <pb.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include "test.pb.h"

TEST(build_test, build_and_link_test)
{
	IntegerContainer container = { 0 };
	ASSERT_EQ(IntegerContainer_fields[0].tag, IntegerContainer_fields[0].tag);
}

TEST(nanopb, encode_decode_basic_buffer)
{
	uint8_t buffer[512];
	IntegerContainer source = { 0 };
	IntegerContainer destination = { 0 };

	source.a = 1;
	source.has_b = true;
	source.b = 2;

	auto ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	ASSERT_TRUE(pb_encode(&ostream, IntegerContainer_fields, &source));

	auto istream = pb_istream_from_buffer(buffer, ostream.bytes_written);
	ASSERT_TRUE(pb_decode(&istream, IntegerContainer_fields, &destination));

	ASSERT_EQ(source.a, destination.a);
	ASSERT_EQ(source.has_b, destination.has_b);
	ASSERT_EQ(source.b, destination.b);
}


