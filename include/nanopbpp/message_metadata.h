#pragma once
#include "extension_metadata_set.h"
#include "extension.h"

namespace nanopbpp
{

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
	typedef T message_t;

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

}
