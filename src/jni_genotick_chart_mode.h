
#pragma once

#include "jni_helpers.h"
#include "jni_enum.h"

namespace jni {
namespace genotick {

struct SChartModeTagType { static constexpr auto Name() { return "com/alphatica/genotick/chart/GenoChartMode"; } };

class CChartMode : public CDerivedEnum<SChartModeTagType>
{
public:
	using TValueMethod = jni::Method<TagType, jni::jint()>;

#define GENOTICK_CHARTMODE_METHODS(f) \
	f(TValueMethod, value) \

	explicit CChartMode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
		GENOTICK_CHARTMODE_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
	}

	inline typename TValueMethod::ReturnType value(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_value);
	}

private:
	GENOTICK_CHARTMODE_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace genotick
} // namespace jni
