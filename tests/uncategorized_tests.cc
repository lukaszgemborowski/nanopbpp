#include <gtest/gtest.h>

#include <vector>

#include <nanopbpp/basic_decoder.h>
#include <nanopbpp/basic_encoder.h>
#include <nanopbpp/meta_decoder.h>
#include <nanopbpp/meta_encoder.h>
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

TEST(uncategorized_tests, encode_decode_with_meta_helpers)
{
	std::vector<uint8_t> buffer(256);

	IntegerContainer src {0}, dst {0};

	src.a = 3;
	src.has_b = true;
	src.b = 6;

	ASSERT_TRUE(nanopbpp::create_encoder(buffer.begin(), buffer.end(), messages_metadata).encode(src));
	ASSERT_TRUE(nanopbpp::create_decoder(buffer.begin(), buffer.end(), messages_metadata).decode(dst));

	ASSERT_EQ(src.a, dst.a);
	ASSERT_EQ(src.has_b, dst.has_b);
	ASSERT_EQ(src.b, dst.b);
}

TEST(uncategorized_tests, encode_decode_with_extension_and_helpers)
{
	std::vector<uint8_t> buffer(256);
	IntegerContainer source_field = { 0 };
	IntegerContainer destination_field = { 0 };

	source_field.a = 2;
	source_field.has_b = true;
	source_field.b = 3;

	Extendable source = { 0 };
	Extendable destination = { 0 };

	pb_extension_t ext = { 0 };
	ext.type = &field_a;
	ext.dest = &source_field;
	ext.next = nullptr;
	ext.found = false;

	source.extensions = &ext;

	ASSERT_TRUE(nanopbpp::create_encoder(buffer.begin(), buffer.end(), messages_metadata).encode(source));

	ext.type = &field_a;
	ext.next = nullptr;
	ext.found = false;
	ext.dest = &destination_field;
	destination.extensions = &ext;

	ASSERT_TRUE(nanopbpp::create_decoder(buffer.begin(), buffer.end(), messages_metadata).decode(destination));

	ASSERT_EQ(source_field.a, destination_field.a);
	ASSERT_EQ(source_field.has_b, destination_field.has_b);
	ASSERT_EQ(source_field.b, destination_field.b);
}
