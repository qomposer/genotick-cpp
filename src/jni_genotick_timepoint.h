
#pragma once

#include "jni_helpers.h"

namespace jni {
namespace genotick {

class CTimePoint
{
public:
	struct TagType { static constexpr auto Name() { return "com/alphatica/genotick/timepoint/TimePoint"; } };
	using TClass = jni::Class<TagType>;
	using TUniqueClass = jni::UniqueClass<TagType>;
	using TObject = jni::Object<TagType>;

	using TTimePointConstructor = jni::Constructor<TagType, jni::jlong>;
	using TGetValueMethod = jni::Method<TagType, jni::jlong()>;

#define GENOTICK_TIMEPOINT_METHODS(f) \
	f(TGetValueMethod, getValue) \

	explicit CTimePoint(jni::JNIEnv* pJavaEnv)
		: m_javaEnv(*pJavaEnv)
		, m_uniqueClass(TClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
		, m_constructor(m_uniqueClass->GetConstructor<jni::jlong>(m_javaEnv))
		GENOTICK_TIMEPOINT_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
	}

	TObject New(jni::jlong value) const
	{
		return m_uniqueClass->New(m_javaEnv, m_constructor, value);
	}

	jni::jlong getValue(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_getValue);
	}

private:
	jni::JNIEnv& m_javaEnv;
	TUniqueClass m_uniqueClass;
	TTimePointConstructor m_constructor;
	GENOTICK_TIMEPOINT_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace genotick
} // namespace jni
