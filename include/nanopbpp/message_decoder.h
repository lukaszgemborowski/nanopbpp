#pragma once

#include "basic_decoder.h"
#include "message.h"

namespace nanopbpp
{

template<typename It, typename M>
class message_decoder : public basic_decoder<It>
{
public:
	message_decoder(It begin, It end) :
		basic_decoder<It>(begin, end)
	{
	}

	bool decode(M &destination)
	{
		return basic_decoder<It>::decode(destination.fields(), &destination.raw());
	}
};

template<typename It, typename M>
message_decoder<It, M> create_decoder(It begin, It end)
{
	return message_decoder<It, M>(begin, end);
}

template<typename It, typename M>
bool decode(It begin, It end, M &destination)
{
	return create_decoder<It, M>(begin, end).decode(destination);
}

}
