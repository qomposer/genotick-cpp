
#pragma once

#include <cstdio>
#include <cstdint>
#include <cassert>
#include <string>
#include <algorithm>

namespace utils
{
template<typename T, unsigned int size>
unsigned int GetArraySize(T(&)[size])
{
	return size;
}

template <class T>
void VerifyFunctionsStruct(const T& instance)
{
	const size_t size = sizeof(T) / sizeof(::std::uintptr_t);
	for (int i = 0; i < size; ++i)
	{
		auto ptr = reinterpret_cast<const ::std::uintptr_t*>(&instance) + i;
		assert(*ptr != 0);
	}
}
} // namespace utils

namespace stl
{
template<typename ... Args>
inline ::std::string string_format(const char* format, Args ... args)
{
	size_t size = ::std::snprintf(nullptr, 0, format, args ...) + 1;
	::std::unique_ptr<char[]> buf(new char[size]);
	::std::snprintf(buf.get(), size, format, args ...);
	return ::std::string(buf.get(), buf.get() + size - 1);
}

template<class Container, class Value>
inline bool find_and_erase(Container& container, const Value& value)
{
	typename Container::iterator it = ::std::find(container.begin(), container.end(), value);
	if (it != container.end())
	{
		container.erase(it);
		return true;
	}
	return false;
}

template<class Container, class Predicate>
inline bool find_and_erase_if(Container& container, const Predicate& predicate)
{
	typename Container::iterator it = ::std::find_if(container.begin(), container.end(), predicate);
	if (it != container.end())
	{
		container.erase(it);
		return true;
	}
	return false;
}

template<class Iterator, class Value, class Compare = std::less<>>
inline Iterator binary_find(Iterator first, Iterator last, const Value& value, Compare comp = {})
{
	first = ::std::lower_bound(first, last, value, comp);
	return (first != last) && !comp(value, *first) ? first : last;
}

template<class Iterator, class Value, class Compare = std::less<>>
inline typename Iterator::difference_type binary_find_index(Iterator first, Iterator last, const Value& value, Compare comp = {})
{
	Iterator it = binary_find(first, last, value, comp);
	return ::std::distance(first, it);
}

} // namespace stl
