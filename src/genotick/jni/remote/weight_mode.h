
#pragma once

#include <genotick/jni/remote/enum.h>
#include <genotick/interface.h>

namespace genotick {
namespace jni {
namespace remote {

struct SWeightModeTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/WeightMode"; } };

class CWeightMode : public CEnum<SWeightModeTag, CGenotickWeightMode>
{
public:
	explicit CWeightMode(::jni::JNIEnv& javaEnv)
		: CEnum<TagType, TCppEnum>(javaEnv)
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
