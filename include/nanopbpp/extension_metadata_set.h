#pragma once
#include <tuple>
#include "extension_metadata.h"

namespace nanopbpp
{

template<typename... E>
struct extension_metadata_set
{
	extension_metadata_set(E... exts) : extensions(std::make_tuple(exts...)) {}
	std::tuple<E...> extensions;

	using extensions_tuple_t = std::tuple<E...>;
};

}
