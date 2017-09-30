
#pragma once

#include <jni/functions.hpp>
#include <jni/errors.hpp>
#include <jni/types.hpp>
#include <exception>
#include <igenotick.h>

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

inline EGenotickResult HandleJavaException(jni::JNIEnv& env, const PendingJavaException& exception)
{
	(void)exception;
	// TODO get and store exception description
	//jni::ThrowableObject throwableObj(jni::ExceptionOccurred(env));
	jni::ExceptionDescribe(env);
	jni::ExceptionClear(env);
	return EGenotickResult::JavaException;
}

inline EGenotickResult HandleEnumMismatchException(const EnumMismatchException& exception)
{
	(void)exception;
	// TODO store and print exception
	return EGenotickResult::JavaEnumMismatch;
}

} // namespace jni
