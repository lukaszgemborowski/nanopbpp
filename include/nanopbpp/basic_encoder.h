#pragma once

#include <tuple>
#include <pb.h>
#include <pb_encode.h>
#include <nanopbpp/metadata.h>
#include <nanopbpp/cpp/get_by_type.h>

namespace nanopbpp
{

template<typename It>
class basic_encoder
{
public:
	basic_encoder(It begin, It end) :
		current (begin),
		ostream {&basic_encoder<It>::callback, this, static_cast<size_t>(std::distance(begin, end))}
	{
	}

	bool encode(const pb_field_t fields[], const void *src_struct)
	{
		return pb_encode(&ostream, fields, src_struct);
	}

	int size() const
	{
		return ostream.bytes_written;
	}

private:
	static bool callback(pb_ostream_t *stream, const uint8_t *buf, size_t count)
	{
		auto encoder = reinterpret_cast<basic_encoder<It> *>(stream->state);

		for (size_t i = 0; i < count; i ++) {
			*encoder->current ++ = buf[i];
		}

		return true;
	}

private:
	It current;
	pb_ostream_t ostream;
};

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
	bool encode(T &source)
	{
		return basic_encoder<It>::encode(cpp::get_by_type<message_metadata<T>>(meta).fields, &source);
	}

private:
	Meta &meta;
};

template<typename It>
basic_encoder<It> create_encoder(It begin, It end)
{
	return basic_encoder<It>(begin, end);
}

template<typename It, typename Meta>
meta_encoder<It, Meta> create_encoder(It begin, It end, Meta &meta)
{
	return meta_encoder<It, Meta>(begin, end, meta);
}

} // namespace nanopbpp
