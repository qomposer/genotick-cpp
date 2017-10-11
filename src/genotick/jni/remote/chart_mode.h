
#pragma once

#include <genotick/jni/remote/enum.h>
#include <genotick/interface.h>

namespace genotick {
namespace jni {
namespace wrapper {

struct SChartModeTag { static constexpr auto Name() { return "com/alphatica/genotick/chart/GenoChartMode"; } };

class CChartMode : public CEnum<SChartModeTag, EGenotickChartMode>
{
public:
	using TValueMethod = ::jni::Method<TagType, ::jni::jint()>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TValueMethod, value) \

	explicit CChartMode(::jni::JNIEnv* pJavaEnv)
		: CEnum<TagType, TEnumClass>(pJavaEnv)
		GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
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
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace wrapper
} // namespace jni
} // namespace genotick
