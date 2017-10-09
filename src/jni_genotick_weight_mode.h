
#pragma once

#include "jni_enum.h"
#include "genotick.h"

namespace jni {
namespace genotick {

struct SWeightModeTagType { static constexpr auto Name() { return "com/alphatica/genotick/genotick/WeightMode"; } };

class CWeightMode : public CDerivedEnum<SWeightModeTagType, EGenotickWeightMode>
{
public:
	explicit CWeightMode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
	{
		VerifyEnumValues();
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
