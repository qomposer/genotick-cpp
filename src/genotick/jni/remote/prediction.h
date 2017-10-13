
#pragma once

#include <genotick/jni/remote/enum.h>
#include <genotick/interface.h>

namespace genotick {
namespace jni {
namespace remote {

struct SPredictionTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/Prediction"; } };

class CPrediction : public CEnum<SPredictionTag, EGenotickPrediction>
{
public:
	using TGetValueMethod = ::jni::Method<TagType, ::jni::jint()>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TGetValueMethod, getValue) \

	explicit CPrediction(::jni::JNIEnv& javaEnv)
		: CEnum<TagType, TEnumClass>(javaEnv)
		GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
		VerifyEnumValues();
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
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace remote
} // namespace jni
} // namespace genotick
