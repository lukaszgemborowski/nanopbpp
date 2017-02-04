#include <gtest/gtest.h>
#include <pb.h>
#include "test.pb.h"

TEST(build_test, build_and_link_test)
{
	IntegerContainer container = { 0 };
	ASSERT_EQ(IntegerContainer_fields[0].tag, IntegerContainer_fields[0].tag);
}
