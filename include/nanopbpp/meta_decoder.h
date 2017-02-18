#pragma once

#include "basic_decoder.h"
#include "message_metadata_set.h"

namespace nanopbpp
{

template<typename It, typename Meta>
class meta_decoder : public basic_decoder<It>
{
public:
	meta_decoder(It begin, It end, Meta &types) :
		basic_decoder<It>(begin, end),
		meta (types)
	{
	}

	template<typename T>
	bool decode(T &source)
	{
		return basic_decoder<It>::decode(meta.template fields<T>(), &source);
	}

private:
	Meta &meta;
};

template<typename It, typename Meta>
meta_decoder<It, Meta> create_decoder(It begin, It end, Meta &meta)
{
	return meta_decoder<It, Meta>(begin, end, meta);
}

}
