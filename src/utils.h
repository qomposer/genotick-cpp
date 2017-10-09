
#pragma once

#include <cstdio>
#include <string>
#include <algorithm>

namespace utils
{
	template<typename T, unsigned int size>
	unsigned int GetArrayLength(T(&)[size])
	{
		return size;
	}
}

namespace stl
{
	template<typename ... Args>
	inline std::string string_format(const char* format, Args ... args)
	{
		size_t size = std::snprintf(nullptr, 0, format, args ...) + 1;
		std::unique_ptr<char[]> buf(new char[size]);
		std::snprintf(buf.get(), size, format, args ...);
		return std::string(buf.get(), buf.get() + size - 1);
	}

	template<class Container, class Value>
	inline bool find_and_erase(Container& container, const Value& value)
	{
		typename Container::iterator it = std::find(container.begin(), container.end(), value);
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
		typename Container::iterator it = std::find_if(container.begin(), container.end(), predicate);
		if (it != container.end())
		{
			container.erase(it);
			return true;
		}
		return false;
	}
}
