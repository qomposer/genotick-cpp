
#pragma once

#include "jni_helpers.h"
#include "jni_enum.h"

namespace jni {
namespace genotick {

struct SWeightModeTagType { static constexpr auto Name() { return "com/alphatica/genotick/genotick/WeightMode"; } };

class CWeightMode : public CDerivedEnum<SWeightModeTagType>
{
public:
	explicit CWeightMode(jni::JNIEnv* pJavaEnv)
		: CDerivedEnum(pJavaEnv)
	{
	}
};

} // namespace genotick
} // namespace jni
