
#pragma once

#include <exception>
#include <iostream>
#include <genotick/interface.h>
#include <genotick/jni/jni_helpers.h>

namespace genotick {
namespace jni {

class EnumMismatchException : public std::exception
{
public:
	explicit EnumMismatchException(char const* const message)
		: std::exception(message)
	{}

	explicit EnumMismatchException(const std::string& message)
		: EnumMismatchException(message.c_str())
	{}
};

class UnrecognizedThreadException : public std::exception
{
public:
	explicit UnrecognizedThreadException(char const* const message)
		: std::exception(message)
	{}

	explicit UnrecognizedThreadException(const std::string& message)
		: UnrecognizedThreadException(message.c_str())
	{}
};

inline EGenotickResult HandleJavaException(::jni::JNIEnv& env, const ::jni::PendingJavaException&)
{
	//jni::ThrowableObject throwableObj(jni::ExceptionOccurred(env));
	::jni::ExceptionDescribe(env);
	::jni::ExceptionClear(env);
	return EGenotickResult::JavaException;
}

inline EGenotickResult HandleEnumMismatchException(const EnumMismatchException& exception)
{
	std::cout << exception.what() << std::endl;
	return EGenotickResult::JavaEnumMismatch;
}

inline EGenotickResult HandleUnrecognizedThreadException(const UnrecognizedThreadException& exception)
{
	std::cout << exception.what() << std::endl;
	return EGenotickResult::ThreadNotAttached;
}

} // namespace jni
} // namespace genotick
