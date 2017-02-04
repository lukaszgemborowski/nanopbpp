#include <gtest/gtest.h>

#include <vector>
#include <nanopbpp/basic_encoder.h>
#include <nanopbpp/meta_encoder.h>
#include <pb_decode.h>

#include "meta.h"

TEST(basic_encoder_tests, encoding_into_vector)
{
	std::vector<uint8_t> buffer(512);
	IntegerContainer source = { 0 };
	IntegerContainer destination = { 0 };

	source.a = 1;
	source.has_b = true;
	source.b = 2;

	auto encoder = nanopbpp::create_encoder(buffer.begin(), buffer.end());
	encoder.encode(IntegerContainer_fields, &source);

	auto istream = pb_istream_from_buffer(buffer.data(), encoder.size());
	ASSERT_TRUE(pb_decode(&istream, IntegerContainer_fields, &destination));

	ASSERT_EQ(source.a, destination.a);
	ASSERT_EQ(source.has_b, destination.has_b);
	ASSERT_EQ(source.b, destination.b);
}

TEST(basic_encoder_tests, vector_to_small)
{
	std::vector<uint8_t> buffer(2);
	IntegerContainer source = { 0 };

	source.a = 1;
	source.has_b = true;
	source.b = 2;

	auto encoder = nanopbpp::create_encoder(buffer.begin(), buffer.end());
	ASSERT_FALSE(encoder.encode(IntegerContainer_fields, &source));
}

TEST(basic_encoder_tests, meta_encoder)
{
	std::vector<uint8_t> buffer(512);
	IntegerContainer source = { 0 };
	IntegerContainer destination = { 0 };

	source.a = 1;
	source.has_b = true;
	source.b = 2;

	auto encoder = nanopbpp::create_encoder(buffer.begin(), buffer.end(), messages_metadata);
	encoder.encode(source);

	auto istream = pb_istream_from_buffer(buffer.data(), encoder.size());
	ASSERT_TRUE(pb_decode(&istream, IntegerContainer_fields, &destination));

	ASSERT_EQ(source.a, destination.a);
	ASSERT_EQ(source.has_b, destination.has_b);
	ASSERT_EQ(source.b, destination.b);
}
