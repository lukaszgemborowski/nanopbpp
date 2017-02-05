#pragma once
#include "cpp/get_by_type.h"
#include <type_traits>
#include <pb.h>

namespace nanopbpp
{

template<size_t TAG>
class base_extension
{
public:
	static constexpr size_t EXTENSION_TAG = TAG;

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
class extension_with_storage : public base_extension<TAG>
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

template<typename... E>
class extension_set
{
	template<size_t TAG, int I = std::tuple_size<std::tuple<E...>>::value - 1>
	struct tag_to_index
	{
		constexpr static auto value = std::tuple_element<I, std::tuple<E...>>::type::EXTENSION_TAG == TAG ? I : tag_to_index<TAG, I - 1>::value;
	};

	template<size_t TAG>
	struct tag_to_index<TAG, -1>
	{
		constexpr static auto value = -1;
	};

public:
	extension_set(E... extensions) :
		extensions (extensions...)
	{
	}

	template<typename U>
	void attach(U &message)
	{
		loop_attach<U, std::tuple_size<std::tuple<E...>>::value>(message);
	}

	template<size_t I, typename T = typename std::tuple_element<I, std::tuple<E...>>::type>
	T& get_by_index()
	{
		return std::get<I>(extensions);
	}

	template<size_t TAG, typename T = typename std::tuple_element<tag_to_index<TAG>::value, std::tuple<E...>>::type >
	T& get_by_tag()
	{
		return std::get<tag_to_index<TAG>::value>(extensions);
	}

private:
	template<typename U, size_t N>
	typename std::enable_if<N == 0, void>::type
	loop_attach(U &) {}

	template<typename U, size_t N>
	typename std::enable_if<N != 0, void>::type
	loop_attach(U &message)
	{
		std::get<N - 1>(extensions).attach(message);
		loop_attach<U, N-1>(message);
	}


private:
	std::tuple<E...> extensions;
};

template<typename... E> extension_set<E...> make_extension_set(E... extensions)
{
	return extension_set<E...>(extensions...);
}

}
