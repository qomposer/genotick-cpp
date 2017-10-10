
#pragma once

#include "jni_enum.h"
#include "genotick.h"

namespace jni {
namespace genotick {

struct SInheritedWeightModeTag { static constexpr auto Name() { return "com/alphatica/genotick/breeder/InheritedWeightMode"; } };

class CInheritedWeightMode : public CEnum<SInheritedWeightModeTag, EGenotickInheritedWeightMode>
{
public:
	explicit CInheritedWeightMode(jni::JNIEnv* pJavaEnv)
		: CEnum<TagType, TEnumClass>(pJavaEnv)
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
