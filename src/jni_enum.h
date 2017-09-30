
#pragma once

#include "jni_helpers.h"
#include "jni_exceptions.h"
#include "utils.h"

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

	virtual jni::jint GetEnumValue(const TObject& object) const = 0;

	typename TNameMethod::ReturnType name(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_name);
	}

	typename TOrdinalMethod::ReturnType ordinal(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_ordinal);
	}

	typename TGetDeclaringClassMethod::ReturnType getDeclaringClass(const TObject& object) const
	{
		return object.Call(m_javaEnv, m_getDeclaringClass);
	}

	typename TValuesMethod::ReturnType values() const
	{
		return m_uniqueClass->Call(m_javaEnv, m_values);
	}

	typename TValueOfMethod::ReturnType valueOf(const jni::String& enumName) const
	{
		return m_uniqueClass->Call(m_javaEnv, m_valueOf, enumName);
	}

protected:
	template <class NativeEnumValue>
	void VerifyEnumValue(NativeEnumValue nativeValue, const char* javaValue)
	{
		const jni::String javaValueString = jni::Make<jni::String>(m_javaEnv, javaValue);
		const TObject javaEnumObject = this->valueOf(javaValueString);
		const jni::jint expectedValue = static_cast<jni::jint>(nativeValue);
		const jni::jint actualValue = this->GetEnumValue(javaEnumObject);
		if (expectedValue != actualValue)
		{
			throw EnumMismatchException(stl::string_format(
				"Enum value '%s' of enum class '%s' does not match. Expected: %d. Actual: %d.",
				javaValue, TagType::Name(), expectedValue, actualValue));
		}
	}

	void VerifyEnumValueCount(jni::jsize expectedLength)
	{
		const TObjectArray values = this->values();
		const jni::jsize actualLength = values.Length(m_javaEnv);
		if (expectedLength != actualLength)
		{
			throw jni::EnumMismatchException(stl::string_format(
				"Enum value count of enum class '%s' does not match. Expected: %d. Actual: %d.",
				TagType::Name(), expectedLength, actualLength));
		}
	}

	jni::JNIEnv& m_javaEnv;
	TUniqueClass m_uniqueClass;
	JAVA_ENUM_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
	JAVA_ENUM_STATIC_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace jni
