#include <gtest/gtest.h>

#include <vector>

#include <nanopbpp/basic_decoder.h>
#include <nanopbpp/basic_encoder.h>
#include "meta.h"

TEST(uncategorized_tests, encode_decode_with_helpers)
{
	std::vector<uint8_t> buffer(512);

	IntegerContainer src {0}, dst {0};

	src.a = 2;
	src.has_b = true;
	src.b = 8;

	auto encoder = nanopbpp::create_encoder(buffer.begin(), buffer.end());
	ASSERT_TRUE(encoder.encode(IntegerContainer_fields, &src));

	auto decoder = nanopbpp::create_decoder(buffer.begin(), buffer.end());
	ASSERT_TRUE(decoder.decode(IntegerContainer_fields, &dst));

	ASSERT_EQ(src.a, dst.a);
	ASSERT_EQ(src.has_b, dst.has_b);
	ASSERT_EQ(src.b, dst.b);
}
