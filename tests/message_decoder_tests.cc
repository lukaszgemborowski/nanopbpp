#include <gtest/gtest.h>
#include <nanopbpp/message_decoder.h>

#include "meta.h"

TEST(message_decoder_tests, decode_message)
{
	IntegerContainer src = {0};

	src.a = 2;
	src.has_b = true;
	src.b = 5;

	uint8_t buffer[64] = {0};
	auto ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	ASSERT_TRUE(pb_encode(&ostream, IntegerContainer_fields, &src));

	auto dst = messages_metadata.create<IntegerContainer>();
	ASSERT_TRUE(decode(buffer, buffer+sizeof(buffer), dst));

	ASSERT_EQ(src.a, dst->a);
	ASSERT_EQ(src.has_b, dst->has_b);
	ASSERT_EQ(src.b, dst->b);
}	
