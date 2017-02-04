#pragma once

namespace nanopbpp
{
namespace cpp
{
namespace detail
{

template <class T, std::size_t N, class... Args>
struct get_number_of_element_from_tuple_by_type_impl
{
	static constexpr auto value = N;
};

template <class T, std::size_t N, class... Args>
struct get_number_of_element_from_tuple_by_type_impl<T, N, T, Args...>
{
	static constexpr auto value = N;
};

template <class T, std::size_t N, class U, class... Args>
struct get_number_of_element_from_tuple_by_type_impl<T, N, U, Args...>
{
	static constexpr auto value = get_number_of_element_from_tuple_by_type_impl<T, N + 1, Args...>::value;
};

} // namespace detail

template <class T, class... Args>
T get_by_type(const std::tuple<Args...>& t)
{
	return std::get<detail::get_number_of_element_from_tuple_by_type_impl<T, 0, Args...>::value>(t);
}

}
}
