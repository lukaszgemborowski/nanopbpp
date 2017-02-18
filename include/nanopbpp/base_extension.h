#pragma once

#include <pb.h>

namespace nanopbpp
{

template<size_t TAG>
class base_extension
{
public:
	static constexpr size_t EXTENSION_TAG = TAG;

	base_extension(const pb_extension_type_t &original) :
		definition (original)
	{
	}

	template<typename U>
	void attach_to_message(pb_extension_t &extension, U &message) const
	{
		extension.type = &definition;

		if (message.extensions) {
			extension.next = message.extensions;
			message.extensions = &extension;
		} else {
			extension.next = nullptr;
			message.extensions = &extension;
		}
	}

	template<typename T>
	void attach_to_storage(pb_extension_t &extension, T &storage) const
	{
		extension.dest = &storage;
	}

protected:
	pb_extension_type_t definition;
};

}
