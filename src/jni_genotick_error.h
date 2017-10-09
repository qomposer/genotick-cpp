
#pragma once

#include "jni_enum.h"
#include "genotick.h"

namespace jni {
namespace genotick {

struct SGenotickErrorCodeMeta
{
	constexpr SGenotickErrorCodeMeta(const char* const javaValueName, const EGenotickResult result)
		: javaValueName(javaValueName)
		, result(result) {}
	const char* const javaValueName;
	const EGenotickResult result;
};

#define GENOTICK_ENUM_ERROR_CODE(f) \
	f(NoError         , = 0 , (SGenotickErrorCodeMeta("NO_ERROR"        , EGenotickResult::ErrorNoError        ))) \
	f(NoInput         , = 1 , (SGenotickErrorCodeMeta("NO_INPUT"        , EGenotickResult::ErrorNoInput        ))) \
	f(NoOutput        , = 2 , (SGenotickErrorCodeMeta("NO_OUTPUT"       , EGenotickResult::ErrorNoOutput       ))) \
	f(UnknownArgument , = 3 , (SGenotickErrorCodeMeta("UNKNOWN_ARGUMENT", EGenotickResult::ErrorUnknownArgument))) \

DEFINE_CUSTOM_ENUM_CLASS(EErrorCode, jni::jint, GENOTICK_ENUM_ERROR_CODE, SGenotickErrorCodeMeta)

inline EGenotickResult ErrorCodeToGenotickResult(const jni::jint error)
{
	return EErrorCode::getByValue(error).meta().result;
}

struct SErrorCodeTagType { static constexpr auto Name() { return "com/alphatica/genotick/genotick/ErrorCode"; } };

class CErrorCode : public CDerivedEnum<SErrorCodeTagType, EErrorCode>
{
public:
	using TValueMethod = jni::Method<TagType, jni::jint()>;

#define GENOTICK_ERRORCODE_METHODS(f) \
	f(TValueMethod, getValue) \

	explicit CErrorCode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
		GENOTICK_ERRORCODE_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
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
