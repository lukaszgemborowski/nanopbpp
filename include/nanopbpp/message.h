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
	message(M &meta) : meta (meta) {}

	const typename M::message_t* operator->() const
	{
		return &storage;
	}

	typename M::message_t* operator->()
	{
		return &storage;
	}

private:
	M &meta;
	typename M::message_t storage;
};

template<typename M>
message<M> make_message(M &meta)
{
	return message<M>(meta);
}

}
