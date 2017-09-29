
#pragma once

#include "jni_helpers.h"
#include "jni_enum.h"

namespace jni {
namespace genotick {

struct SInheritedWeightModeTagType { static constexpr auto Name() { return "com/alphatica/genotick/breeder/InheritedWeightMode"; } };

class CInheritedWeightMode : public CDerivedEnum<SInheritedWeightModeTagType>
{
public:
	explicit CInheritedWeightMode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
	{
	}
};

} // namespace genotick
} // namespace jni
