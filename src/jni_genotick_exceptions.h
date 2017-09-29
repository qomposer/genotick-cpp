
#pragma once

#include <exception>

namespace jni {
namespace genotick {

class EnumMismatchException : public std::exception
{
public:
	explicit EnumMismatchException(char const* const message)
		: std::exception(message)
	{}
};

} // namespace genotick
} // namespace jni
