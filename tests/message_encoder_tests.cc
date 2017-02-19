#include <gtest/gtest.h>
#include <nanopbpp/message_encoder.h>
#include <pb_decode.h>

#include "meta.h"

TEST(message_encoder_tests, encode_message)
{
	auto src = messages_metadata.create<IntegerContainer>();
	src->a = 2;
	src->has_b = true;
	src->b = 3;

	uint8_t buffer[64] = {0};

	ASSERT_TRUE(encode(buffer, buffer + sizeof(buffer), src));

	auto istream = pb_istream_from_buffer(buffer, sizeof(buffer));
	IntegerContainer dst = {0};
	ASSERT_TRUE(pb_decode(&istream, IntegerContainer_fields, &dst));

	ASSERT_EQ(src->a, dst.a);
	ASSERT_EQ(src->has_b, dst.has_b);
	ASSERT_EQ(src->b, dst.b);
}
