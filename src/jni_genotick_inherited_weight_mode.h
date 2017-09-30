
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
		VerifyEnumValue(EGenotickInheritedWeightMode::Parents, "PARENTS");
		VerifyEnumValue(EGenotickInheritedWeightMode::Ancestors, "ANCESTORS");
		VerifyEnumValue(EGenotickInheritedWeightMode::AncestorsLog, "ANCESTORS_LOG");
		VerifyEnumValueCount(3);
	}

	jni::jint GetEnumValue(const TObject& object) const override final
	{
		return ordinal(object);
	}
};

} // namespace genotick
} // namespace jni
