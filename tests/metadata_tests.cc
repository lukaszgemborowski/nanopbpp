#include <gtest/gtest.h>

#include <nanopbpp/metadata.h>
#include "test.pb.h"

TEST(metadata_tests, instantiation)
{
	using namespace nanopbpp;

	message_metadata_set<
		message_metadata<IntegerContainer>,
		message_metadata<FloatContainer>>
	messageset(
		IntegerContainer_fields,
		FloatContainer_fields);

	auto m = messageset.get_by_meta_type<IntegerContainer>();

	auto same = std::is_same<message_metadata<IntegerContainer>, decltype(m)>::value;
	ASSERT_TRUE(same);
}
