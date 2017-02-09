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

TEST(nanopb, encode_decode_empty_message_in_container)
{
	uint8_t buffer[512];
	Container source = { 0 };
	Container destination = { 0 };

	source.has_field_a = true;

	auto ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	ASSERT_TRUE(pb_encode(&ostream, Container_fields, &source));

	EXPECT_NE(0, ostream.bytes_written);

	auto istream = pb_istream_from_buffer(buffer, ostream.bytes_written);
	ASSERT_TRUE(pb_decode(&istream, Container_fields, &destination));
}


TEST(nanopb, encode_decode_with_extension)
{
	uint8_t buffer[256];
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

	auto ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	ASSERT_TRUE(pb_encode(&ostream, Extendable_fields, &source));

	ext.type = &field_a;
	ext.next = nullptr;
	ext.found = false;
	ext.dest = &destination_field;
	destination.extensions = &ext;

	auto istream = pb_istream_from_buffer(buffer, ostream.bytes_written);
	ASSERT_TRUE(pb_decode(&istream, Extendable_fields, &destination));

	ASSERT_EQ(source_field.a, destination_field.a);
	ASSERT_EQ(source_field.has_b, destination_field.has_b);
	ASSERT_EQ(source_field.b, destination_field.b);
}

struct context {
	context(bool &value) : value (value) {}

	bool &value;
};

bool nanopb_tests_decode_1(pb_istream_t *stream, pb_extension_t *extension, uint32_t tag, pb_wire_type_t wire_type)
{
	context *ctx = (context *)extension->dest;
	ctx->value = true;
	return true;
}

TEST(nanopb, BUG_encode_decode_extension)
{
	uint8_t buffer[256];
	IntegerContainer source_field = { 0 };
	Extendable source = { 0 };

	pb_extension_t source_ext = { 0 };
	source_ext.type = &field_a;
	source_ext.dest = &source_field; // all zeroes - this is important
	source_ext.next = nullptr;
	source_ext.found = false;

	source.extensions = &source_ext;

	auto ostream = pb_ostream_from_buffer(buffer, sizeof(buffer));
	ASSERT_TRUE(pb_encode(&ostream, Extendable_fields, &source));

	EXPECT_NE(0, ostream.bytes_written);
}
