#pragma once

#include <tuple>
#include <pb.h>
#include "extension.h"
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

template<typename T, typename E = extension_metadata_set<> >
struct message_metadata
{
private:
	template<typename U>
	auto base_to_instance(U&& t) {
		using base_type = std::remove_const_t<std::remove_reference_t<U>>;
		return extension<base_type::tag, typename base_type::type_t>(t.extension_type_def);
	}

	template<typename U, size_t... Is>
	auto instantiate_tuple_of_extensions(U&& t, std::index_sequence<Is...>) {
		return std::make_tuple(base_to_instance(std::get<Is>(t))...);
	}

public:
	typedef E extensions_set_t;

	message_metadata(const pb_field_t *fields, E e = extension_metadata_set<>())
		: fields (fields), extensions (e) {}

	// this requires C++14 support
	auto instantiate_extensions() {
		using seq = std::make_index_sequence<std::tuple_size<std::decay_t<typename E::extensions_tuple_t>>::value>;
		return make_extension_set(instantiate_tuple_of_extensions(std::forward<typename E::extensions_tuple_t>(extensions.extensions), seq{}));
	}

	const pb_field_t *fields;
	E extensions;
};

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
