
#pragma once

#include <genotick/interface.h>
#include <jni.h>

namespace genotick {
namespace jni {

inline EGenotickResult JniErrorToGenotickResult(jint error)
{
	switch (error)
	{
	case JNI_OK: return EGenotickResult::Success;
	case JNI_ERR: default: return EGenotickResult::JniError;
	case JNI_EDETACHED: return EGenotickResult::JniDetached;
	case JNI_EVERSION: return EGenotickResult::JniVersionMismatch;
	case JNI_ENOMEM: return EGenotickResult::JniNoMemory;
	case JNI_EEXIST: return EGenotickResult::JniExists;
	case JNI_EINVAL: return EGenotickResult::JniInvalidArgument;
	}
}

} // namespace jni
} // namespace genotick
