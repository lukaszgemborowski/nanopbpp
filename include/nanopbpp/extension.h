#pragma once
#include <pb.h>

namespace nanopbpp
{

template<typename T>
class extension_with_storage
{
public:
	extension_with_storage(const pb_extension_type_t &extension) :
		extension_def (extension)
	{
	}

	T& value()
	{
		return storage;
	}

	const T& value() const
	{
		return storage;
	}

	template<typename U>
	void attach(U &message)
	{
		extension.found = false;
		extension.dest = &storage;
		extension.type = &extension_def;
		extension.next = nullptr;

		if (message.extensions) {
			extension.next = message.extensions;
			message.extensions = &extension;
		} else {
			message.extensions = &extension;
		}
	}

private:
	T storage;
	const pb_extension_type_t &extension_def;
	pb_extension_t extension;
};

}
