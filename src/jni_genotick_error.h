
#pragma once

#include "jni_enum.h"
#include <igenotick.h>

namespace jni {
namespace genotick {

#define GENOTICK_ENUM_ERROR_CODE(f) \
	f(eErrorCode_NoError         , NO_ERROR        ,  0, EGenotickResult::eGenotickResult_ErrorNoError        ) \
	f(eErrorCode_NoInput         , NO_INPUT        ,  1, EGenotickResult::eGenotickResult_ErrorNoInput        ) \
	f(eErrorCode_NoOutput        , NO_OUTPUT       ,  2, EGenotickResult::eGenotickResult_ErrorNoOutput       ) \
	f(eErrorCode_UnknownArgument , UNKNOWN_ARGUMENT,  3, EGenotickResult::eGenotickResult_ErrorUnknownArgument) \

GENOTICK_DEFINE_ENUM_WITH_VALUE_COUNT(EErrorCode, GENOTICK_ENUM_ERROR_CODE, eErrorCode_Count)

#define GENOTICK_UNROLL_ERROR_CODE_RESULT_MAPPING(a,b,c,result) result,

inline EGenotickResult ErrorCodeToGenotickResult(const jni::jint error)
{
	if (error < eErrorCode_Count)
	{
		const EGenotickResult results[] = { GENOTICK_ENUM_ERROR_CODE(GENOTICK_UNROLL_ERROR_CODE_RESULT_MAPPING) };
		return results[error];
	}
	throw EnumMismatchException(stl::string_format("Unknown error code with value %d", error));
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
		GENOTICK_ENUM_ERROR_CODE(GENOTICK_UNROLL_VERIFY_ENUM_VALUE);
		VerifyEnumValueCount(EErrorCode::eErrorCode_Count);
	}

	jni::jint value(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_getValue);
	}

	jni::jint GetEnumValue(const TObject& object) const override final
	{
		return value(object);
	}

	TObject GetEnumObject(const jni::jint value) const override final
	{
		return GetEnumObjectBySearch(value);
	}

private:
	GENOTICK_ERRORCODE_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace genotick
} // namespace jni
