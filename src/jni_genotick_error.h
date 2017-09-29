
#pragma once

#include "jni_helpers.h"
#include <igenotick.h>
#include <assert.h>

// For simplicity sake we use 'jint' instead of 'enum ErrorCode'

namespace jni {
namespace genotick {

enum EErrorCode : jni::jint
{
	CONTINUE = -1,
	NO_ERROR = 0,
	NO_INPUT = 1,
	NO_OUTPUT = 2,
	UNKNOWN_ARGUMENT = 3,
};

inline EGenotickResult ErrorCodeToGenotickResult(const jni::jint error)
{
	switch (error)
	{
	case EErrorCode::CONTINUE: return EGenotickResult::ErrorContinue;
	case EErrorCode::NO_ERROR: return EGenotickResult::ErrorNoError;
	case EErrorCode::NO_INPUT: return EGenotickResult::ErrorNoInput;
	case EErrorCode::NO_OUTPUT: return EGenotickResult::ErrorNoOutput;
	case EErrorCode::UNKNOWN_ARGUMENT: return EGenotickResult::ErrorUnknownArgument;
	default: assert(0); return EGenotickResult::ErrorUnknown;
	}
}

} // namespace genotick
} // namespace jni
