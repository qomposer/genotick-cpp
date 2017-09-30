
#pragma once

#include "jni_enum.h"
#include <igenotick.h>

namespace jni {
namespace genotick {

enum class EErrorCode : jni::jint
{
	Continue = -1,
	NoError = 0,
	NoInput = 1,
	NoOutput = 2,
	UnknownArgument = 3,
};

inline EGenotickResult ErrorCodeToGenotickResult(const jni::jint error)
{
	switch (static_cast<EErrorCode>(error))
	{
	case EErrorCode::Continue: return EGenotickResult::ErrorContinue;
	case EErrorCode::NoError: return EGenotickResult::ErrorNoError;
	case EErrorCode::NoInput: return EGenotickResult::ErrorNoInput;
	case EErrorCode::NoOutput: return EGenotickResult::ErrorNoOutput;
	case EErrorCode::UnknownArgument: return EGenotickResult::ErrorUnknownArgument;
	default: throw EnumMismatchException(stl::string_format("Unhandled error code %d", error));
	}
}

struct SErrorCodeTagType { static constexpr auto Name() { return "com/alphatica/genotick/genotick/ErrorCode"; } };

class CErrorCode : public CDerivedEnum<SErrorCodeTagType>
{
public:
	using TValueMethod = jni::Method<TagType, jni::jint()>;

#define GENOTICK_ERRORCODE_METHODS(f) \
	f(TValueMethod, getValue) \

	explicit CErrorCode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
		GENOTICK_ERRORCODE_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
		VerifyEnumValue(EErrorCode::Continue, "CONTINUE");
		VerifyEnumValue(EErrorCode::NoError, "NO_ERROR");
		VerifyEnumValue(EErrorCode::NoInput, "NO_INPUT");
		VerifyEnumValue(EErrorCode::NoOutput, "NO_OUTPUT");
		VerifyEnumValue(EErrorCode::UnknownArgument, "UNKNOWN_ARGUMENT");
		VerifyEnumValueCount(5);
	}

	jni::jint value(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_getValue);
	}

	jni::jint GetEnumValue(const TObject& object) const override final
	{
		return value(object);
	}

private:
	GENOTICK_ERRORCODE_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace genotick
} // namespace jni
