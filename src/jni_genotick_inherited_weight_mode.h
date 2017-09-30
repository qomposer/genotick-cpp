
#pragma once

#include "jni_enum.h"
#include <igenotick.h>

namespace jni {
namespace genotick {

struct SInheritedWeightModeTagType { static constexpr auto Name() { return "com/alphatica/genotick/breeder/InheritedWeightMode"; } };

class CInheritedWeightMode : public CDerivedEnum<SInheritedWeightModeTagType>
{
public:
	explicit CInheritedWeightMode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
	{
		GENOTICK_ENUM_INHERITED_WEIGHT_MODE(GENOTICK_UNROLL_VERIFY_ENUM_VALUE);
		VerifyEnumValueCount(EGenotickInheritedWeightMode::eGenotickInheritedWeightMode_Count);
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
