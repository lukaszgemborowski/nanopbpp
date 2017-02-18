#pragma once

namespace nanopbpp
{

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
