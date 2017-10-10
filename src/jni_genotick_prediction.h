
#pragma once

#include "jni_enum.h"
#include "genotick.h"

namespace jni {
namespace genotick {

struct SPredictionTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/Prediction"; } };

class CPrediction : public CEnum<SPredictionTag, EGenotickPrediction>
{
public:
	using TValueMethod = jni::Method<TagType, jni::jint()>;

#define GENOTICK_PREDICTION_METHODS(f) \
	f(TValueMethod, getValue) \

	explicit CPrediction(jni::JNIEnv* pJavaEnv)
		: CEnum<TagType, TEnumClass>(pJavaEnv)
		GENOTICK_PREDICTION_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
		VerifyEnumValues();
	}

	jni::jint value(const TObject& object) const
	{
		return object.Call(GetJavaEnv(), m_getValue);
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
	GENOTICK_PREDICTION_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_PREDICTION_METHODS

} // namespace genotick
} // namespace jni
