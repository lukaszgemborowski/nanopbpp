#include <gtest/gtest.h>

#include <vector>

#include <nanopbpp/basic_decoder.h>
#include <nanopbpp/basic_encoder.h>
#include <nanopbpp/meta_decoder.h>
#include <nanopbpp/meta_encoder.h>
#include <nanopbpp/extension.h>
#include <nanopbpp/extension_set.h>
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

TEST(uncategorized_tests, extension_set_usage)
{
	std::vector<uint8_t> buffer(256);
	IntegerContainer srca = {0}, dsta = {0};
	FloatContainer srcb = {0}, dstb = {0};

	auto source_set = nanopbpp::make_extension_set(
		nanopbpp::extension<field_a_tag, IntegerContainer>(field_a),
		nanopbpp::extension<field_b_tag, FloatContainer>(field_b)
	);

	auto destination_set = source_set;

	srca.a = 2;
	srca.has_b = true;
	srca.b = 3;
	srcb.c = 4.5f;

	Extendable source = {0}, destination = {0};

	source_set.get_by_tag<field_a_tag>().attach_to_storage(srca);
	source_set.get_by_tag<field_b_tag>().attach_to_storage(srcb);
	source_set.attach(source);

	ASSERT_TRUE(nanopbpp::create_encoder(buffer.begin(), buffer.end(), messages_metadata).encode(source));

	destination_set.get_by_tag<field_a_tag>().attach_to_storage(dsta);
	destination_set.get_by_tag<field_b_tag>().attach_to_storage(dstb);
	destination_set.attach(destination);
	ASSERT_TRUE(nanopbpp::create_decoder(buffer.begin(), buffer.end(), messages_metadata).decode(destination));

	ASSERT_EQ(srca.a, dsta.a);
	ASSERT_EQ(srca.b, dsta.b);
	ASSERT_EQ(srcb.c, dstb.c);
}

TEST(uncategorized_tests, instatiate_tuple_of_extensions_from_metadata)
{
	std::vector<uint8_t> buffer(256);
	IntegerContainer srca = {0}, dsta = {0};
	FloatContainer srcb = {0}, dstb = {0};
	Extendable srcmsg = {0}, dstmsg = {0};
	auto extensions = messages_metadata.get_by_meta_type<Extendable>().instantiate_extensions();

	extensions.get_by_tag<field_a_tag>().attach_to_storage(srca);
	extensions.get_by_tag<field_b_tag>().attach_to_storage(srcb);

	srca.a = 1;
	srca.has_b = true;
	srca.b = 2;
	srcb.c = 3.4f;

	extensions.attach(srcmsg);

	ASSERT_TRUE(nanopbpp::create_encoder(buffer.begin(), buffer.end(), messages_metadata).encode(srcmsg));

	extensions.get_by_tag<field_a_tag>().attach_to_storage(dsta);
	extensions.get_by_tag<field_b_tag>().attach_to_storage(dstb);
	extensions.attach(dstmsg);

	ASSERT_TRUE(nanopbpp::create_decoder(buffer.begin(), buffer.end(), messages_metadata).decode(dstmsg));

	ASSERT_EQ(srca.a, dsta.a);
	ASSERT_EQ(srca.b, dsta.b);
	ASSERT_EQ(srcb.c, dstb.c);

}
