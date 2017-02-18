#pragma once

#include <tuple>
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

template<typename... E>
struct extensions_metadata_set
{
	extensions_metadata_set(E... exts) : extensions(std::make_tuple(exts...)) {}
	std::tuple<E...> extensions;
};

template<typename T, typename E = extensions_metadata_set<> >
struct message_metadata
{
	typedef E extensions_set_t;

	message_metadata(const pb_field_t *fields, E e = extensions_metadata_set<>())
		: fields (fields), extensions (e) {}

	const pb_field_t *fields;
	E extensions;
};

template<typename... M>
class messages_metadata_set
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

	messages_metadata_set(M... messages) : messages(std::make_tuple(messages...)) {}
	std::tuple<M...> messages;
};

}
