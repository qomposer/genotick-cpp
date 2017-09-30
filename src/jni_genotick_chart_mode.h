
#pragma once

#include "jni_enum.h"
#include <igenotick.h>

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
		VerifyEnumValue(EGenotickChartMode::None, "NONE");
		VerifyEnumValue(EGenotickChartMode::Draw, "DRAW");
		VerifyEnumValue(EGenotickChartMode::Save, "SAVE");
		VerifyEnumValue(EGenotickChartMode::JFreeChart, "JFREECHART");
		VerifyEnumValue(EGenotickChartMode::JFreeChart_Draw, "JFREECHART_DRAW");
		VerifyEnumValue(EGenotickChartMode::JFreeChart_Save, "JFREECHART_SAVE");
		VerifyEnumValue(EGenotickChartMode::JFreeChart_Draw_Save, "JFREECHART_DRAW_SAVE");
		VerifyEnumValueCount(7);
	}

	jni::jint value(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_value);
	}

	jni::jint GetEnumValue(const TObject& object) const override final
	{
		return value(object);
	}

private:
	GENOTICK_CHARTMODE_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace genotick
} // namespace jni
