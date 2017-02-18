#pragma once

#include "basic_encoder.h"
#include "message.h"

namespace nanopbpp
{

template<typename It, typename M>
class message_encoder : public basic_encoder<It>
{
public:
	message_encoder(It begin, It end) :
		basic_encoder<It>(begin, end)
	{
	}

	bool encode(const M &source)
	{
		return basic_encoder<It>::encode(source.fields(), &source.raw());
	}
};

template<typename It, typename M>
message_encoder<It, M> create_encoder(It begin, It end)
{
	return message_encoder<It, M>(begin, end);
}

template<typename It, typename M>
bool encode(It begin, It end, const M &source)
{
	return create_encoder<It, M>(begin, end).encode(source);
}

}
