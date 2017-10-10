
#pragma once

#include "jni_enum.h"
#include "genotick.h"

namespace jni {
namespace genotick {

struct SPredictionTagType { static constexpr auto Name() { return "com/alphatica/genotick/genotick/Prediction"; } };

class CPrediction : public CDerivedEnum<SPredictionTagType, EGenotickPrediction>
{
public:
	using TValueMethod = jni::Method<TagType, jni::jint()>;

#define GENOTICK_PREDICTION_METHODS(f) \
	f(TValueMethod, getValue) \

	explicit CPrediction(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
		GENOTICK_PREDICTION_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
		VerifyEnumValues();
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
	GENOTICK_PREDICTION_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace genotick
} // namespace jni
