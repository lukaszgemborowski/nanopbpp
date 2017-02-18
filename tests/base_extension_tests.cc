#include <gtest/gtest.h>
#include <nanopbpp/base_extension.h>

namespace
{
	const size_t tag = 1;
	pb_extension_type_t ext_type = {0};

	struct dummy_message { int a; };
}

TEST(base_extension_tests, instantiation)
{
	nanopbpp::base_extension<tag> extension(ext_type);
}

TEST(base_extension_tests, check_if_storage_is_attached)
{
	pb_extension_t ext = {0};
	dummy_message msg = {0};
	nanopbpp::base_extension<tag> extension(ext_type);

	ASSERT_FALSE(extension.has_storage_attached(ext));

	extension.attach_to_storage(ext, msg);

	ASSERT_TRUE(extension.has_storage_attached(ext));
}
