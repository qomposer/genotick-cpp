
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
	case EErrorCode::CONTINUE: return eGenotickResult_ErrorContinue;
	case EErrorCode::NO_ERROR: return eGenotickResult_ErrorNoError;
	case EErrorCode::NO_INPUT: return eGenotickResult_ErrorNoInput;
	case EErrorCode::NO_OUTPUT: return eGenotickResult_ErrorNoOutput;
	case EErrorCode::UNKNOWN_ARGUMENT: return eGenotickResult_ErrorUnknownArgument;
	default: assert(0); return eGenotickResult_ErrorUnknown;
	}
}

} // namespace genotick
} // namespace jni