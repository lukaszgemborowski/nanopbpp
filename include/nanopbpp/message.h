#pragma once
#include <pb.h>
#include <pb_encode.h>
#include "extension_set.h"

namespace nanopbpp
{

template<typename M>
class message
{
public:
	message(M &meta) :
		meta (meta),
		extensions (meta.instantiate_extensions())
	{
		uint8_t *begin = reinterpret_cast<uint8_t *>(&storage);
		uint8_t *end = begin + sizeof(storage);
		std::fill(begin, end, 0);

		extensions.attach(storage);
	}

	const typename M::message_t* operator->() const
	{
		return &storage;
	}

	typename M::message_t* operator->()
	{
		return &storage;
	}

	const pb_field_t *fields() const
	{
		return meta.fields;
	}

	typename M::message_t& raw()
	{
		return storage;
	}

	const typename M::message_t& raw() const
	{
		return storage;
	}

	template<size_t TAG>
	auto& extension_by_tag()
	{
		return extensions.template get_by_tag<TAG>();
	}

private:
	M &meta;
	decltype(meta.instantiate_extensions()) extensions;
	typename M::message_t storage;
};

template<typename M>
message<M> make_message(M &meta)
{
	return message<M>(meta);
}

}
