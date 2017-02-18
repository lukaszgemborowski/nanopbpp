#pragma once
#include <pb.h>

namespace nanopbpp
{

template<size_t TAG, typename T>
struct extension_metadata
{
	extension_metadata(const pb_extension_type_t &def) :
		extension_type_def (def)
	{}

	const pb_extension_type_t &extension_type_def;

	static constexpr auto tag = TAG;
	using type_t = T;
};

}
