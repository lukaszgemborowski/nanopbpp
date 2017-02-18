#include <gtest/gtest.h>
#include <nanopbpp/extension.h>

namespace
{
	const size_t tag = 1;
	pb_extension_type_t ext_type = {0};

	struct dummy_message { int a; };
}

TEST(extension_tests, instantiation)
{
	nanopbpp::extension<tag, dummy_message> extension(ext_type);
}

TEST(extension_tests, check_if_storage_is_attached)
{
	dummy_message msg;

	nanopbpp::extension<tag, dummy_message> extension(ext_type);
	ASSERT_FALSE(extension.has_storage_attached());

	extension.attach_to_storage(msg);
	ASSERT_TRUE(extension.has_storage_attached());
}
