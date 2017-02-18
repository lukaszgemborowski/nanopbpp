#pragma once

namespace nanopbpp
{

template<typename T>
class message
{
public:
	const T* operator->() const
	{
		return &storage;
	}

	T* operator->()
	{
		return &storage;
	}

private:
	T storage;
};

}
