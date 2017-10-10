
#pragma once

#include "jni_genotick_settings.h"

namespace jni {
namespace genotick {

struct SMainInterfaceTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/MainInterface"; } };

class CMainInterface : public CClass<SMainInterfaceTag>
{
public:
	using TGetInterfaceVersionMethod = jni::StaticMethod<TagType, jni::jint()>;
	using TStartMethod = jni::StaticMethod<TagType, jni::jint(jni::jint /* sessionId */, jni::StringArray /* args */)>;
	using TGetSettingsMethod = jni::StaticMethod<TagType, CMainSettings::TObject(jni::jint /* sessionId */)>;

#define GENOTICK_CLASS_STATIC_METHODS(f) \
	f(TGetInterfaceVersionMethod, getInterfaceVersion) \
	f(TStartMethod              , start              ) \
	f(TGetSettingsMethod        , getSettings        ) \

	explicit CMainInterface(jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		GENOTICK_CLASS_STATIC_METHODS(GENOTICK_UNROLL_STATIC_METHOD_INITIALIZERS)
	{}

	jni::jint getInterfaceVersion() const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_getInterfaceVersion);
	}

	jni::jint start(const jni::jint sessionId, const jni::StringArray& array) const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_start, sessionId, array);
	}

	CMainSettings::TObject getSettings(const jni::jint sessionId) const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_getSettings, sessionId);
	}

private:
	GENOTICK_CLASS_STATIC_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_STATIC_METHODS

} // namespace genotick
} // namespace jni
