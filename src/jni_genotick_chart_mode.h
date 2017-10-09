
#pragma once

#include "jni_enum.h"
#include "genotick.h"

namespace jni {
namespace genotick {

struct SChartModeTagType { static constexpr auto Name() { return "com/alphatica/genotick/chart/GenoChartMode"; } };

class CChartMode : public CDerivedEnum<SChartModeTagType, EGenotickChartMode>
{
public:
	using TValueMethod = jni::Method<TagType, jni::jint()>;

#define GENOTICK_CHART_MODE_METHODS(f) \
	f(TValueMethod, value) \

	explicit CChartMode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
		GENOTICK_CHART_MODE_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
		VerifyEnumValues();
	}

	jni::jint value(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_value);
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
	GENOTICK_CHART_MODE_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace genotick
} // namespace jni
