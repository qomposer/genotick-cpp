
#pragma once

#include "jni_enum.h"
#include <igenotick.h>

namespace jni {
namespace genotick {

struct SWeightModeTagType { static constexpr auto Name() { return "com/alphatica/genotick/genotick/WeightMode"; } };

class CWeightMode : public CDerivedEnum<SWeightModeTagType>
{
public:
	explicit CWeightMode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
	{
		VerifyEnumValue(EGenotickWeightMode::WinCount, "WIN_COUNT");
		VerifyEnumValue(EGenotickWeightMode::WinRate, "WIN_RATE");
		VerifyEnumValue(EGenotickWeightMode::ProfitCount, "PROFIT_COUNT");
		VerifyEnumValue(EGenotickWeightMode::ProfitFactor, "PROFIT_FACTOR");
		VerifyEnumValueCount(4);
	}

	jni::jint GetEnumValue(const TObject& object) const override final
	{
		return ordinal(object);
	}
};

} // namespace genotick
} // namespace jni
