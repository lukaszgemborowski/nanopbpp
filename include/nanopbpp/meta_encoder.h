#pragma once
#include "basic_encoder.h"
#include <nanopbpp/message_metadata_set.h>

namespace nanopbpp
{

template<typename It, typename Meta>
class meta_encoder : public basic_encoder<It>
{
public:
	meta_encoder(It begin, It end, Meta &types) :
		basic_encoder<It>(begin, end),
		meta (types)
	{
	}

	template<typename T>
	bool encode(const T &source)
	{
		return basic_encoder<It>::encode(meta. template fields<T>(), &source);
	}

private:
	Meta &meta;
};

template<typename It, typename Meta>
meta_encoder<It, Meta> create_encoder(It begin, It end, Meta &meta)
{
	return meta_encoder<It, Meta>(begin, end, meta);
}

template<typename It, typename Meta, typename T>
bool encode(Meta &meta, It begin, It end, const T &source)
{
	return create_encoder(begin, end, meta).encode(source);
}

}
