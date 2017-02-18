#pragma once
#include <type_traits>
#include <functional>
#include <pb.h>
#include "base_extension.h"

namespace nanopbpp
{

template<size_t TAG, typename T>
class extension : public base_extension<TAG>
{
public:
	extension(const pb_extension_type_t &original) : base_extension<TAG>(original)
	{
	}

	void attach_to_storage(T &storage)
	{
		base_extension<TAG>::attach_to_storage(pb_extension, storage);
	}

	template<typename M>
	void attach_to_message(M &message)
	{
		base_extension<TAG>::attach_to_message(pb_extension, message);
	}

	template<typename M>
	void attach(M &message)
	{
		attach_to_message(message);
	}

	template<typename M>
	void attach(M &message, T& storage)
	{
		attach_to_message(message);
		attach_to_storage(storage);
	}

private:
	pb_extension_t pb_extension;
};

template<size_t TAG>
class callback_extension : public base_extension<TAG>
{
public:
	callback_extension(const pb_extension_type_t &extension) :
		base_extension<TAG> (extension),
		callback(),
		extension {0},
		fake {0} // this fake is importat, it needs to be 0-ed out (especially tag field)
	{
		this->definition.decode = &callback_extension<TAG>::decode;
		this->definition.encode = nullptr;
		this->definition.arg = &fake;
	}

	template<typename U, typename F>
	void attach(U &message, const F &func)
	{
		this->attach_to_message(extension, message);
		extension.type = &this->definition;
		extension.dest = this;
		callback = func;
	}

private:
	static bool decode(pb_istream_t *stream, pb_extension_t *extension, uint32_t tag, pb_wire_type_t wire_type)
	{
		if (tag != TAG)
			return true;

		callback_extension<TAG> *self = reinterpret_cast<callback_extension<TAG> *>(extension->dest);
		self->callback();
		return true;
	}

private:
	std::function<void ()> callback;
	pb_extension_t extension;
	pb_field_t fake;
};

}
