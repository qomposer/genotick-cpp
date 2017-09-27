
#pragma once

#include <igenotick.h>
#include <jni.h>

namespace jni
{
	inline EGenotickResult JniErrorToGenotickResult(jint error)
	{
		switch (error)
		{
		case JNI_OK: return eGenotickResult_Success;
		case JNI_ERR: default: return eGenotickResult_JniError;
		case JNI_EDETACHED: return eGenotickResult_JniDetached;
		case JNI_EVERSION: return eGenotickResult_JniVersionMismatch;
		case JNI_ENOMEM: return eGenotickResult_JniNoMemory;
		case JNI_EEXIST: return eGenotickResult_JniExists;
		case JNI_EINVAL: return eGenotickResult_JniInvalidArgument;
		}
	}
}
