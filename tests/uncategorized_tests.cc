#include <gtest/gtest.h>

#include <vector>

#include <nanopbpp/basic_decoder.h>
#include <nanopbpp/basic_encoder.h>
#include <nanopbpp/meta_decoder.h>
#include <nanopbpp/meta_encoder.h>
#include <nanopbpp/extension.h>
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

TEST(uncategorized_tests, simple_extension_usage)
{
	std::vector<uint8_t> buffer(256);

	IntegerContainer source_storage = { 0 }, destination_storage = { 0 };
	nanopbpp::simple_extension<field_a_tag> source_ext(field_a);
	nanopbpp::simple_extension<field_a_tag> destination_ext(field_a);

	source_storage.a = 2;
	source_storage.has_b = true;
	source_storage.b = 4;

	Extendable source = { 0 }, destination = { 0 };

	source_ext.attach(source, source_storage);

	ASSERT_TRUE(nanopbpp::create_encoder(buffer.begin(), buffer.end(), messages_metadata).encode(source));

	destination_ext.attach(destination, destination_storage);

	ASSERT_TRUE(nanopbpp::create_decoder(buffer.begin(), buffer.end(), messages_metadata).decode(destination));

	ASSERT_EQ(source_storage.a, destination_storage.a);
	ASSERT_EQ(source_storage.has_b, destination_storage.has_b);
	ASSERT_EQ(source_storage.b, destination_storage.b);
}

TEST(uncategorized_tests, extension_helper)
{
	std::vector<uint8_t> buffer(256);

	nanopbpp::extension_with_storage<field_a_tag, IntegerContainer> source_ext(field_a);
	nanopbpp::extension_with_storage<field_a_tag, IntegerContainer> destination_ext(field_a);

	source_ext.value().a = 2;
	source_ext.value().has_b = true;
	source_ext.value().b = 4;

	Extendable source = { 0 }, destination = { 0 };

	source_ext.attach(source);

	ASSERT_TRUE(nanopbpp::create_encoder(buffer.begin(), buffer.end(), messages_metadata).encode(source));

	destination_ext.attach(destination);

	ASSERT_TRUE(nanopbpp::create_decoder(buffer.begin(), buffer.end(), messages_metadata).decode(destination));

	ASSERT_EQ(source_ext.value().a, destination_ext.value().a);
	ASSERT_EQ(source_ext.value().has_b, destination_ext.value().has_b);
	ASSERT_EQ(source_ext.value().b, destination_ext.value().b);
}

TEST(uncategorized_tests, extension_set_usage)
{
	std::vector<uint8_t> buffer(256);

	auto source_set = nanopbpp::make_extension_set(
		nanopbpp::extension_with_storage<field_a_tag, IntegerContainer>(field_a),
		nanopbpp::extension_with_storage<field_b_tag, FloatContainer>(field_b)
	);

	auto destination_set = source_set;

	source_set.get_by_tag<field_a_tag>().value().a = 2;
	source_set.get_by_tag<field_a_tag>().value().has_b = true;
	source_set.get_by_tag<field_a_tag>().value().b = 3;
	source_set.get_by_tag<field_b_tag>().value().c = 4.5f;

	Extendable source = {0}, destination = {0};

	source_set.attach(source);
	ASSERT_TRUE(nanopbpp::create_encoder(buffer.begin(), buffer.end(), messages_metadata).encode(source));

	destination_set.attach(destination);
	ASSERT_TRUE(nanopbpp::create_decoder(buffer.begin(), buffer.end(), messages_metadata).decode(destination));

	ASSERT_EQ(source_set.get_by_index<0>().value().a, destination_set.get_by_index<0>().value().a);
	ASSERT_EQ(source_set.get_by_index<0>().value().has_b, destination_set.get_by_index<0>().value().has_b);
	ASSERT_EQ(source_set.get_by_index<0>().value().b, destination_set.get_by_index<0>().value().b);
	ASSERT_EQ(source_set.get_by_index<1>().value().c, destination_set.get_by_index<1>().value().c);
}

TEST(uncategorized_tests, callback_extension)
{
	std::vector<uint8_t> buffer(256);
	bool callback_called = false;

	IntegerContainer source_storage = { 0 };
	nanopbpp::simple_extension<field_a_tag> source_ext(field_a);
	nanopbpp::callback_extension<field_a_tag> destination_ext;

	source_storage.a = 2;
	source_storage.has_b = true;
	source_storage.b = 4;

	Extendable source = { 0 }, destination = { 0 };

	source_ext.attach(source, source_storage);

	ASSERT_TRUE(nanopbpp::create_encoder(buffer.begin(), buffer.end(), messages_metadata).encode(source));

	destination_ext.attach(destination, [&callback_called]() { callback_called = true; });

	ASSERT_TRUE(nanopbpp::create_decoder(buffer.begin(), buffer.end(), messages_metadata).decode(destination));
	ASSERT_TRUE(callback_called);
}
