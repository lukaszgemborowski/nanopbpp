#include <gtest/gtest.h>
#include <nanopbpp/base_extension.h>

TEST(base_extension_tests, instantiation)
{
	pb_extension_type_t ext_type = {0};
	const size_t tag = 1;

	nanopbpp::base_extension<tag> extension(ext_type);
}
