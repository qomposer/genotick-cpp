
#pragma once

#include "jni_helpers.h"
#include "jni_exceptions.h"
#include "utils.h"

namespace jni {

template <class Tag, class Enum>
class CDerivedEnum
{
public:
	using TagType = Tag;
	using TEnumClass = Enum;
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
		VerifyEnumBasics();
	}

	virtual jni::jint GetEnumValue(const TObject& object) const = 0;
	virtual TObject GetEnumObject(const jni::jint value) const = 0;

	jni::String name(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_name);
	}

	jni::jint ordinal(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_ordinal);
	}

	jni::ClassObject getDeclaringClass(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_getDeclaringClass);
	}

	TObjectArray values() const
	{
		return m_uniqueClass->Call(m_javaEnv, m_values);
	}

	TObject valueOf(const jni::String& enumName) const
	{
		return m_uniqueClass->Call(m_javaEnv, m_valueOf, enumName);
	}

protected:
	TObject GetEnumObjectBySearch(const jni::jint value) const;
	TObject GetEnumObjectByOrdinal(const jni::jint ordinal) const;
	void VerifyEnumValues();
	void VerifyEnumValue(jni::jint nativeValue, const char* const javaValueName);

private:
	void VerifyEnumBasics();
	void VerifyEnumOrdinal(jni::jint nativeOrdinal, const char* const javaValueName);
	void VerifyEnumValueCount(typename TEnumClass::ordinal_type nativeValueCount);

protected:
	jni::JNIEnv& m_javaEnv;
	TUniqueClass m_uniqueClass;

private:
	JAVA_ENUM_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
	JAVA_ENUM_STATIC_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace jni

#include "jni_enum_impl.h"
