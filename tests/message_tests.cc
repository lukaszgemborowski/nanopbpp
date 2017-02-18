#include <gtest/gtest.h>
#include <nanopbpp/message.h>

#include "meta.h"

TEST(message_tests, accessing_fields)
{
	using namespace nanopbpp;

	message<IntegerContainer> msg;

	msg->a = 1;
	msg->has_b = true;
	msg->b = 2;

	ASSERT_EQ(1, msg->a);
	ASSERT_EQ(2, msg->b);
	ASSERT_TRUE(msg->has_b);
}
