
#pragma once

#include "jni_helpers.h"
#include "jni_exceptions.h"
#include "utils.h"

namespace jni {

template <class TheTag, class EnumClass>
class CDerivedEnum
{
public:
	using TagType = TheTag;
	using TEnumClass = EnumClass;
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
	TObject GetEnumObjectBySearch(const jni::jint value) const
	{
		const TObjectArray enumObjects = values();
		const jni::jsize length = enumObjects.Length(m_javaEnv);
		for (jni::jsize i = 0; i < length; ++i)
		{
			const TObject enumObject = enumObjects.Get(m_javaEnv, i);
			const jni::jint givenValue = GetEnumValue(enumObject);
			if (value == givenValue)
			{
				return enumObject;
			}
		}
		throw EnumMismatchException(stl::string_format(
			"Enum constant of enum class '%s' with value %d was not found", TagType::Name(), value));
	}

	TObject GetEnumObjectByOrdinal(const jni::jint ordinal) const
	{
		const TObjectArray enumValues = values();
		const jni::jsize length = enumValues.Length(m_javaEnv);
		if (static_cast<jni::jsize>(ordinal) < length) {
			return enumValues.Get(m_javaEnv, ordinal);
		}
		throw EnumMismatchException(stl::string_format(
			"Enum constant of enum class '%s' with ordinal %d was not found", TagType::Name(), ordinal));
	}

protected:
	void VerifyEnumValues()
	{
		const TEnumClass::ordinal_type count = TEnumClass::count();
		for (TEnumClass::ordinal_type i = 0; i < count; ++i)
		{
			const TEnumClass& instance = TEnumClass::getByOrdinal(i);
			VerifyEnumValue(instance.value(), instance.meta().javaValueName);
		}
	}

	void VerifyEnumValue(jni::jint nativeValue, const char* const javaValueName)
	{
		const jni::String javaValueString = jni::Make<jni::String>(m_javaEnv, javaValueName);
		const TObject enumObject = valueOf(javaValueString);
		const jni::jint javaValue = GetEnumValue(enumObject);
		if (nativeValue != javaValue)
		{
			throw EnumMismatchException(stl::string_format(
				"Enum value of '%s' of enum class '%s' does not match. Expected: %d. Actual: %d.",
				javaValueName, TagType::Name(), nativeValue, javaValue));
		}
	}

private:
	void VerifyEnumBasics()
	{
		const TEnumClass::ordinal_type count = TEnumClass::count();
		VerifyEnumValueCount(count);
		for (TEnumClass::ordinal_type i = 0; i < count; ++i)
		{
			const TEnumClass& instance = TEnumClass::getByOrdinal(i);
			VerifyEnumOrdinal(instance.ordinal(), instance.meta().javaValueName);
		}
	}

	void VerifyEnumOrdinal(jni::jint nativeOrdinal, const char* const javaValueName)
	{
		const jni::String javaValueString = jni::Make<jni::String>(m_javaEnv, javaValueName);
		const TObject enumObject = valueOf(javaValueString);
		const jni::jint javaOrdinal = ordinal(enumObject);
		if (nativeOrdinal != javaOrdinal)
		{
			throw EnumMismatchException(stl::string_format(
				"Enum ordinal of '%s' of enum class '%s' does not match. Expected: %d. Actual: %d.",
				javaValueName, TagType::Name(), nativeOrdinal, javaOrdinal));
		}
	}

	void VerifyEnumValueCount(typename TEnumClass::ordinal_type nativeValueCount)
	{
		const TObjectArray enumObjects = values();
		const jni::jsize javaValueCount = enumObjects.Length(m_javaEnv);
		if (nativeValueCount != javaValueCount)
		{
			throw jni::EnumMismatchException(stl::string_format(
				"Enum value count of enum class '%s' does not match. Expected: %d. Actual: %d.",
				TagType::Name(), nativeValueCount, javaValueCount));
		}
	}

protected:
	jni::JNIEnv& m_javaEnv;
	TUniqueClass m_uniqueClass;
	JAVA_ENUM_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
	JAVA_ENUM_STATIC_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

} // namespace jni
