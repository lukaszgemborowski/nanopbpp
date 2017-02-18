#pragma once
#include "message_metadata.h"
#include "message.h"

namespace nanopbpp
{

template<typename... M>
class message_metadata_set
{
	template<typename T, size_t N, typename... Args>
	struct get_by_type_impl
	{
		static constexpr auto value = N;
	};

	template<typename T, size_t N, typename... Args>
	struct get_by_type_impl<T, N,
		message_metadata<T,
				typename std::tuple_element<N, std::tuple<M...>>::type::extensions_set_t
			>,
		Args...>
	{
		static constexpr auto value = N;
	};

	template<typename T, size_t N, typename U, typename... Args>
	struct get_by_type_impl<T, N, U, Args...>
	{
		static constexpr auto value = get_by_type_impl<T, N + 1, Args...>::value;
	};

public:
	template<typename T, typename E = typename std::tuple_element<get_by_type_impl<T, 0, M...>::value, std::tuple<M...>>::type::extensions_set_t>
	message_metadata<T, E>& get_by_meta_type()
	{
		std::get<
			get_by_type_impl<T, 0, M...>::value>(messages);
	}

	template<typename T>
	const pb_field_t *fields()
	{
		return get_by_meta_type<T>().fields;
	}

	message_metadata_set(M... messages) : messages(std::make_tuple(messages...)) {}
	std::tuple<M...> messages;
};

}
