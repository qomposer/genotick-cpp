
#include <utils.h>

namespace genotick {
namespace jni {
namespace remote {

template <class Tag, class Enum>
typename CEnum<Tag, Enum>::TObject CEnum<Tag, Enum>::GetEnumObjectBySearch(const ::jni::jint value) const
{
	const TObjectArray enumObjects = values();
	const ::jni::jsize length = enumObjects.Length(GetJavaEnv());
	for (::jni::jsize i = 0; i < length; ++i)
	{
		const TObject enumObject = enumObjects.Get(GetJavaEnv(), i);
		const ::jni::jint givenValue = GetEnumValue(enumObject);
		if (value == givenValue)
		{
			return enumObject;
		}
	}
	throw EnumMismatchException(::stl::string_format(
		"Enum constant of enum class '%s' with value %d was not found", TagType::Name(), value));
}

template <class Tag, class Enum>
typename CEnum<Tag, Enum>::TObject CEnum<Tag, Enum>::GetEnumObjectByOrdinal(const ::jni::jint ordinal) const
{
	const TObjectArray enumValues = values();
	const ::jni::jsize length = enumValues.Length(GetJavaEnv());
	if (static_cast<::jni::jsize>(ordinal) < length) {
		return enumValues.Get(GetJavaEnv(), ordinal);
	}
	throw EnumMismatchException(::stl::string_format(
		"Enum constant of enum class '%s' with ordinal %d was not found", TagType::Name(), ordinal));
}

template <class Tag, class Enum>
void CEnum<Tag, Enum>::VerifyEnumValues()
{
	const TEnumClass::ordinal_type count = TEnumClass::count();
	for (TEnumClass::ordinal_type i = 0; i < count; ++i)
	{
		const TEnumClass& instance = TEnumClass::get_by_ordinal(i);
		VerifyEnumValue(instance.value(), instance.meta().javaValueName);
	}
}

template <class Tag, class Enum>
void CEnum<Tag, Enum>::VerifyEnumValue(const ::jni::jint nativeValue, const char* const javaValueName)
{
	const ::jni::String javaValueString = ::jni::Make<::jni::String>(GetJavaEnv(), javaValueName);
	const TObject enumObject = valueOf(javaValueString);
	const ::jni::jint javaValue = GetEnumValue(enumObject);
	if (nativeValue != javaValue)
	{
		throw EnumMismatchException(::stl::string_format(
			"Enum value of '%s' of enum class '%s' does not match. Expected: %d. Actual: %d.",
			javaValueName, TagType::Name(), nativeValue, javaValue));
	}
}

template <class Tag, class Enum>
void CEnum<Tag, Enum>::VerifyEnumBasics()
{
	const TEnumClass::ordinal_type count = TEnumClass::count();
	VerifyEnumValueCount(count);
	for (TEnumClass::ordinal_type i = 0; i < count; ++i)
	{
		const TEnumClass& instance = TEnumClass::get_by_ordinal(i);
		VerifyEnumOrdinal(static_cast<::jni::jint>(instance.ordinal()), instance.meta().javaValueName);
	}
}

template <class Tag, class Enum>
void CEnum<Tag, Enum>::VerifyEnumOrdinal(const ::jni::jint nativeOrdinal, const char* const javaValueName)
{
	const ::jni::String javaValueString = ::jni::Make<::jni::String>(GetJavaEnv(), javaValueName);
	const TObject enumObject = valueOf(javaValueString);
	const ::jni::jint javaOrdinal = ordinal(enumObject);
	if (nativeOrdinal != javaOrdinal)
	{
		throw EnumMismatchException(::stl::string_format(
			"Enum ordinal of '%s' of enum class '%s' does not match. Expected: %d. Actual: %d.",
			javaValueName, TagType::Name(), nativeOrdinal, javaOrdinal));
	}
}

template <class Tag, class Enum>
void CEnum<Tag, Enum>::VerifyEnumValueCount(typename TEnumClass::ordinal_type nativeValueCount)
{
	const TObjectArray enumObjects = values();
	const ::jni::jsize javaValueCount = enumObjects.Length(GetJavaEnv());
	if (nativeValueCount != javaValueCount)
	{
		throw EnumMismatchException(::stl::string_format(
			"Enum value count of enum class '%s' does not match. Expected: %d. Actual: %d.",
			TagType::Name(), nativeValueCount, javaValueCount));
	}
}

} // namespace remote
} // namespace jni
} // namespace genotick
