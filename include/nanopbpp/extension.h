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

template<size_t TAG, typename T>
class extension_with_storage : public extension<TAG, T>
{
public:
	extension_with_storage(const pb_extension_type_t &extension) :
		extension<TAG, T> (extension)
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
		extension<TAG, T>::attach_to_message(message);
		extension<TAG, T>::attach_to_storage(storage);
	}

private:
	T storage;
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

	extension_set(const std::tuple<E...> &extensions_tuple) :
		extensions (extensions_tuple)
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

template<typename... E> extension_set<E...> make_extension_set(const std::tuple<E...> &extensions_tuple)
{
	return extension_set<E...>(extensions_tuple);
}

}
