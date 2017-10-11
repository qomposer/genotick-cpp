
#pragma once

#include <genotick/jni/remote/enum.h>
#include <genotick/interface.h>

namespace genotick {
namespace jni {
namespace remote {

struct SInheritedWeightModeTag { static constexpr auto Name() { return "com/alphatica/genotick/breeder/InheritedWeightMode"; } };

class CInheritedWeightMode : public CEnum<SInheritedWeightModeTag, EGenotickInheritedWeightMode>
{
public:
	explicit CInheritedWeightMode(::jni::JNIEnv* pJavaEnv)
		: CEnum<TagType, TEnumClass>(pJavaEnv)
	{
		VerifyEnumValues();
	}

	::jni::jint GetEnumValue(const TObject& object) const override final
	{
		return ordinal(object);
	}

	TObject GetEnumObject(const ::jni::jint value) const override final
	{
		return GetEnumObjectByOrdinal(value);
	}
};

} // namespace remote
} // namespace jni
} // namespace genotick
