
#pragma once

#include <genotick/jni/remote/enum.h>
#include <genotick/interface.h>

namespace genotick {
namespace jni {
namespace remote {

struct SChartModeTag { static constexpr auto Name() { return "com/alphatica/genotick/chart/GenoChartMode"; } };

class CChartMode : public CEnum<SChartModeTag, EGenotickChartMode>
{
public:
	using TValueMethod = ::jni::Method<TagType, ::jni::jint()>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TValueMethod, value) \

	explicit CChartMode(::jni::JNIEnv& javaEnv)
		: CEnum<TagType, TEnumClass>(javaEnv)
	{
		VerifyEnumValues();
	}

	::jni::jint value(const TObject& object) const
	{
		return object.Call(GetJavaEnv(), m_value);
	}

	::jni::jint GetEnumValue(const TObject& object) const override final
	{
		return value(object);
	}

	TObject GetEnumObject(const ::jni::jint value) const override final
	{
		return GetEnumObjectBySearch(value);
	}

private:
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_MEMBERS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace remote
} // namespace jni
} // namespace genotick
