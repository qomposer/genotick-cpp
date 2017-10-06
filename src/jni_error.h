
#pragma once

#include "genotick.h"
#include <jni.h>

namespace jni
{
	inline EGenotickResult JniErrorToGenotickResult(jint error)
	{
		switch (error)
		{
		case JNI_OK: return EGenotickResult::eGenotickResult_Success;
		case JNI_ERR: default: return EGenotickResult::eGenotickResult_JniError;
		case JNI_EDETACHED: return EGenotickResult::eGenotickResult_JniDetached;
		case JNI_EVERSION: return EGenotickResult::eGenotickResult_JniVersionMismatch;
		case JNI_ENOMEM: return EGenotickResult::eGenotickResult_JniNoMemory;
		case JNI_EEXIST: return EGenotickResult::eGenotickResult_JniExists;
		case JNI_EINVAL: return EGenotickResult::eGenotickResult_JniInvalidArgument;
		}
	}
}
