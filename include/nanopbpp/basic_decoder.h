#pragma once

#include <pb.h>
#include <pb_decode.h>

namespace nanopbpp
{

template<typename It>
class basic_decoder
{
public:
	basic_decoder(It begin, It end) :
		current (begin),
		istream {&basic_decoder<It>::callback, this, static_cast<size_t>(std::distance(begin, end))}
	{
	}

	bool decode(const pb_field_t fields[], void *dst_struct)
	{
		return pb_decode(&istream, fields, dst_struct);
	}

private:
	static bool callback(pb_istream_t *stream, uint8_t *buf, size_t count)
	{
		auto decoder = reinterpret_cast<basic_decoder<It> *>(stream->state);

		for (size_t i = 0; i < count; i ++) {
			buf[i] = *decoder->current ++;
		}

		return true;
	}

private:
	It current;
	pb_istream_t istream;
};

template<typename It>
basic_decoder<It> create_decoder(It begin, It end)
{
	return basic_decoder<It>(begin, end);
}

} // namespace nanopbpp
