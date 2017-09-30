
#pragma once

#include "jni_helpers.h"
#include "jni_genotick_settings.h"

namespace jni {
namespace genotick {

class CMainInterface
{
public:
	struct TagType { static constexpr auto Name() { return "com/alphatica/genotick/genotick/MainInterface"; } };
	using TClass = jni::Class<TagType>;
	using TUniqueClass = jni::UniqueClass<TagType>;
	using TObject = jni::Object<TagType>;

	using TGetInterfaceVersionMethod = jni::StaticMethod<TagType, jni::jint()>;
	using TStartMethod = jni::StaticMethod<TagType, jni::jint(jni::StringArray)>;
	using TGetSettingsMethod = jni::StaticMethod<TagType, CMainSettings::TObject()>;

#define GENOTICK_MAININTERFACE_STATIC_METHODS(f) \
	f(TGetInterfaceVersionMethod, getInterfaceVersion) \
	f(TStartMethod              , start              ) \
	f(TGetSettingsMethod        , getSettings        ) \

	explicit CMainInterface(jni::JNIEnv* pJavaEnv)
		: m_javaEnv(*pJavaEnv)
		, m_uniqueClass(TClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
		GENOTICK_MAININTERFACE_STATIC_METHODS(GENOTICK_UNROLL_STATIC_METHOD_INITIALIZERS)
	{}

	jni::jint getInterfaceVersion() const
	{
		return m_uniqueClass->Call(m_javaEnv, m_getInterfaceVersion);
	}

	jni::jint start(const jni::StringArray& array) const
	{
		return m_uniqueClass->Call(m_javaEnv, m_start, array);
	}

	CMainSettings::TObject getSettings() const
	{
		return m_uniqueClass->Call(m_javaEnv, m_getSettings);
	}

private:
	jni::JNIEnv& m_javaEnv;
	TUniqueClass m_uniqueClass;
	GENOTICK_MAININTERFACE_STATIC_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace genotick
} // namespace jni
