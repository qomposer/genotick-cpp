
#pragma once

#include "jni_helpers.h"
#include "jni_genotick_prediction.h"

namespace jni {
namespace genotick {

class CPredictions
{
public:
	struct TagType { static constexpr auto Name() { return "com/alphatica/genotick/genotick/Predictions"; } };
	using TClass = jni::Class<TagType>;
	using TUniqueClass = jni::UniqueClass<TagType>;
	using TObject = jni::Object<TagType>;

	using TGetMethod = jni::Method<TagType, CPrediction::TObject(jni::jint /* index */)>;

#define GENOTICK_PREDICTIONS_METHODS(f) \
	f(TGetMethod, get) \

	explicit CPredictions(jni::JNIEnv* pJavaEnv)
		: m_javaEnv(*pJavaEnv)
		, m_uniqueClass(TClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
		GENOTICK_PREDICTIONS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{}

	CPrediction::TObject get(const TObject& object, jni::jint index) const
	{
		return object.Call(m_javaEnv, m_get, index);
	}

private:
	jni::JNIEnv& m_javaEnv;
	TUniqueClass m_uniqueClass;
	GENOTICK_PREDICTIONS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace genotick
} // namespace jni
