#pragma once

namespace nanopbpp
{

template<typename T>
struct message_metadata
{
	message_metadata(const pb_field_t *fields) : fields (fields) {}

	const pb_field_t *fields;
};

}
