
#pragma once

#include "jni_enum.h"
#include "genotick.h"

namespace jni {
namespace genotick {

struct SWeightModeTagType { static constexpr auto Name() { return "com/alphatica/genotick/genotick/WeightMode"; } };

class CWeightMode : public CDerivedEnum<SWeightModeTagType>
{
public:
	explicit CWeightMode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
	{
		GENOTICK_ENUM_WEIGHT_MODE(GENOTICK_UNROLL_VERIFY_ENUM_VALUE);
		VerifyEnumValueCount(EGenotickWeightMode::eGenotickWeightMode_Count);
	}

	jni::jint GetEnumValue(const TObject& object) const override final
	{
		return ordinal(object);
	}

	TObject GetEnumObject(const jni::jint value) const override final
	{
		return GetEnumObjectByOrdinal(value);
	}
};

} // namespace genotick
} // namespace jni
