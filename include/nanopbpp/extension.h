#pragma once
#include <pb.h>

namespace nanopbpp
{

template<size_t TAG>
class base_extension
{
public:
	base_extension(const pb_extension_type_t &extension) :
		extension_def (extension)
	{
	}

	template<typename U, typename T>
	void attach(U &message, T &storage)
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
	const pb_extension_type_t &extension_def;
	pb_extension_t extension;
};

template<size_t TAG, typename T>
class extension_with_storage : base_extension<TAG>
{
public:
	extension_with_storage(const pb_extension_type_t &extension) :
		base_extension<TAG> (extension)
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
		base_extension<TAG>::attach(message, storage);
	}

private:
	T storage;
};

}
