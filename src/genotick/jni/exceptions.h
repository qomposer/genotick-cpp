
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

inline EGenotickResult HandleJavaException(::jni::JNIEnv& env, const ::jni::PendingJavaException& exception)
{
	(void)exception;
	// TODO get and store exception description
	//jni::ThrowableObject throwableObj(jni::ExceptionOccurred(env));
	::jni::ExceptionDescribe(env);
	::jni::ExceptionClear(env);
	return EGenotickResult::JavaException;
}

inline EGenotickResult HandleEnumMismatchException(const EnumMismatchException& exception)
{
	(void)exception;
	// TODO store and print exception
	std::cout << exception.what() << std::endl;
	return EGenotickResult::JavaEnumMismatch;
}

} // namespace jni
} // namespace genotick
