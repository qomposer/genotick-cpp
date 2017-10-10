
#pragma once

#include "jni_helpers.h"
#include "jni_genotick_prediction.h"

namespace jni {
namespace genotick {

struct SPredictionsTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/Predictions"; } };

class CPredictions : public CClass<SPredictionsTag>
{
public:
	using TGetMethod = jni::Method<TagType, CPrediction::TObject(jni::jint /* index */)>;

#define GENOTICK_PREDICTIONS_METHODS(f) \
	f(TGetMethod, get) \

	explicit CPredictions(jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		GENOTICK_PREDICTIONS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{}

	CPrediction::TObject get(const TObject& object, const jni::jint index) const
	{
		return object.Call(GetJavaEnv(), m_get, index);
	}

private:
	GENOTICK_PREDICTIONS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_PREDICTIONS_METHODS

} // namespace genotick
} // namespace jni
