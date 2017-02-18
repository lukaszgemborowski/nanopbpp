#include "extension.h"
#include "metadata.h"

namespace nanopbpp
{
namespace detail
{
	template<typename T>
	auto base_to_instance(T&& t) {
		using base_type = std::remove_const_t<std::remove_reference_t<T>>;
		return extension<base_type::tag, typename base_type::type_t>(t.extension_type_def);
	}

	template<typename T, size_t... Is>
	auto instantiate_tuple_of_extensions(T&& t, std::index_sequence<Is...>) {
		return std::make_tuple(base_to_instance(std::get<Is>(t))...);
	}
}
 
template<typename T>
auto instantiate_extensions(T&& t) {
	using seq = std::make_index_sequence<std::tuple_size<std::decay_t<T>>::value>;
	return detail::instantiate_tuple_of_extensions(std::forward<T>(t), seq{});
}

}
