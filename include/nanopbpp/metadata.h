#pragma once

#include <tuple>

namespace nanopbpp
{

template<typename T>
struct message_metadata
{
	message_metadata(const pb_field_t *fields) : fields (fields) {}

	const pb_field_t *fields;
};

template<typename... M>
struct messages_metadata_set
{
	messages_metadata_set(M... messages) : messages(std::make_tuple(messages...)) {}
	std::tuple<M...> messages;
};

}
