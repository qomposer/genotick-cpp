
#pragma once

#include <genotick/jni/remote/enum.h>
#include <genotick/interface.h>

namespace genotick {
namespace jni {
namespace remote {

struct SGenotickErrorCodeMeta
{
	constexpr SGenotickErrorCodeMeta(const char* const javaValueName, const EGenotickResult result)
		: javaValueName(javaValueName)
		, result(result) {}
	const char* const javaValueName;
	const EGenotickResult result;
};

#define GENOTICK_ENUM_ERROR_CODE(f) \
	f(NoError          , = 0 , (SGenotickErrorCodeMeta("NO_ERROR"         , EGenotickResult::Success              ))) \
	f(NoInput          , = 1 , (SGenotickErrorCodeMeta("NO_INPUT"         , EGenotickResult::ErrorNoInput         ))) \
	f(NoOutput         , = 2 , (SGenotickErrorCodeMeta("NO_OUTPUT"        , EGenotickResult::ErrorNoOutput        ))) \
	f(UnknownArgument  , = 3 , (SGenotickErrorCodeMeta("UNKNOWN_ARGUMENT" , EGenotickResult::ErrorUnknownArgument ))) \
	f(InvalidSession   , = 4 , (SGenotickErrorCodeMeta("INVALID_SESSION"  , EGenotickResult::ErrorInvalidSession  ))) \
	f(DuplicateSession , = 5 , (SGenotickErrorCodeMeta("DUPLICATE_SESSION", EGenotickResult::ErrorDuplicateSession))) \
	f(InsufficientData , = 6 , (SGenotickErrorCodeMeta("INSUFFICIENT_DATA", EGenotickResult::ErrorInsufficientData))) \

DEFINE_CUSTOM_ENUM_CLASS(EErrorCode, ::jni::jint, GENOTICK_ENUM_ERROR_CODE, SGenotickErrorCodeMeta)

struct SErrorCodeTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/ErrorCode"; } };

class CErrorCode : public CEnum<SErrorCodeTag, EErrorCode>
{
public:
	using TValueMethod = ::jni::Method<TagType, ::jni::jint()>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TValueMethod, getValue) \

	explicit CErrorCode(::jni::JNIEnv& javaEnv)
		: CEnum<TagType, TEnumClass>(javaEnv)
	{
	}

	::jni::jint getValue(const TObject& object) const
	{
		return object.Call(GetJavaEnv(), m_getValue);
	}

	::jni::jint GetEnumValue(const TObject& object) const override final
	{
		return getValue(object);
	}

	TObject GetEnumObject(const ::jni::jint value) const override final
	{
		return GetEnumObjectBySearch(value);
	}

private:
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_MEMBERS)
};

inline EGenotickResult ErrorCodeToGenotickResult(const CErrorCode& errorCode, const CErrorCode::TObject& errorCodeObject)
{
	const ::jni::jint errorCodeValue = errorCode.getValue(errorCodeObject);
	return EErrorCode::get_by_value(errorCodeValue).meta().result;
}

#undef GENOTICK_CLASS_METHODS
#undef GENOTICK_ENUM_ERROR_CODE

} // namespace remote
} // namespace jni
} // namespace genotick
