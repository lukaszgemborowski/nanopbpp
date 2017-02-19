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
	{}

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
