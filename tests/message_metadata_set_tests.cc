#include <gtest/gtest.h>
#include <nanopbpp/message_metadata_set.h>

#include "meta.h"

TEST(message_metadata_set_tests, creating_message)
{
	auto msg = messages_metadata.create<IntegerContainer>();

	using namespace nanopbpp;
	bool result = std::is_same<decltype(msg), message<message_metadata<IntegerContainer, extension_metadata_set<>>>>::value;
	ASSERT_TRUE(result);
}
