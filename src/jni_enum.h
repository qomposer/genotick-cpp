
#pragma once

#include "jni_helpers.h"

namespace jni {

template <class TheTag>
class CDerivedEnum
{
public:
	using TagType = TheTag;
	using TClass = jni::Class<TagType>;
	using TUniqueClass = jni::UniqueClass<TagType>;
	using TObject = jni::Object<TagType>;
	using TObjectArray = jni::Array<TObject>;

	using TNameMethod = jni::Method<TagType, jni::String()>;
	using TOrdinalMethod = jni::Method<TagType, jni::jint()>;
	using TGetDeclaringClassMethod = jni::Method<TagType, jni::ClassObject()>;
	using TValuesMethod = jni::StaticMethod<TagType, TObjectArray()>;
	using TValueOfMethod = jni::StaticMethod<TagType, TObject(jni::String)>;

#define JAVA_ENUM_METHODS(f) \
	f(TNameMethod, name) \
	f(TOrdinalMethod, ordinal) \
	f(TGetDeclaringClassMethod, getDeclaringClass) \

#define JAVA_ENUM_STATIC_METHODS(f) \
	f(TValuesMethod, values) \
	f(TValueOfMethod, valueOf) \

	explicit CDerivedEnum(jni::JNIEnv* pJavaEnv)
		: m_javaEnv(*pJavaEnv)
		, m_uniqueClass(TClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
		JAVA_ENUM_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
		JAVA_ENUM_STATIC_METHODS(GENOTICK_UNROLL_STATIC_METHOD_INITIALIZERS)
	{
	}

	inline typename TNameMethod::ReturnType name(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_name);
	}

	inline typename TOrdinalMethod::ReturnType ordinal(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_ordinal);
	}

	inline typename TGetDeclaringClassMethod::ReturnType getDeclaringClass(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_getDeclaringClass);
	}

	inline typename TValuesMethod::ReturnType values() const
	{
		return m_uniqueClass->Call(m_javaEnv, m_values);
	}

	inline typename TValueOfMethod::ReturnType valueOf(const jni::String& enumName) const
	{
		return m_uniqueClass->Call(m_javaEnv, m_values, enumName);
	}

protected:
	jni::JNIEnv& m_javaEnv;
	TUniqueClass m_uniqueClass;
	JAVA_ENUM_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
	JAVA_ENUM_STATIC_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace jni
