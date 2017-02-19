#pragma once
#include <type_traits>
#include <functional>
#include <pb.h>

namespace nanopbpp
{

template<size_t TAG, typename T>
class extension
{
public:
	static constexpr auto EXTENSION_TAG = TAG;

	extension(const pb_extension_type_t &original) :
		definition(original),
		pb_extension {0}
	{
	}

	void attach_to_storage(T &storage)
	{
		pb_extension.dest = &storage;
	}

	template<typename M>
	void attach_to_message(M &message)
	{
		pb_extension.type = &definition;

		if (message.extensions) {
			pb_extension.next = message.extensions;
			message.extensions = &pb_extension;
		} else {
			pb_extension.next = nullptr;
			message.extensions = &pb_extension;
		}

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

	bool has_storage_attached() const
	{
		return pb_extension.dest != nullptr;
	}

private:
	pb_extension_type_t definition;
	pb_extension_t pb_extension;
};

}
