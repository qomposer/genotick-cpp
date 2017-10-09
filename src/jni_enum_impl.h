
namespace jni {

template <class Tag, class Enum>
typename CDerivedEnum<Tag, Enum>::TObject CDerivedEnum<Tag, Enum>::GetEnumObjectBySearch(const jni::jint value) const
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

template <class Tag, class Enum>
typename CDerivedEnum<Tag, Enum>::TObject CDerivedEnum<Tag, Enum>::GetEnumObjectByOrdinal(const jni::jint ordinal) const
{
	const TObjectArray enumValues = values();
	const jni::jsize length = enumValues.Length(m_javaEnv);
	if (static_cast<jni::jsize>(ordinal) < length) {
		return enumValues.Get(m_javaEnv, ordinal);
	}
	throw EnumMismatchException(stl::string_format(
		"Enum constant of enum class '%s' with ordinal %d was not found", TagType::Name(), ordinal));
}

template <class Tag, class Enum>
void CDerivedEnum<Tag, Enum>::VerifyEnumValues()
{
	const TEnumClass::ordinal_type count = TEnumClass::count();
	for (TEnumClass::ordinal_type i = 0; i < count; ++i)
	{
		const TEnumClass& instance = TEnumClass::getByOrdinal(i);
		VerifyEnumValue(instance.value(), instance.meta().javaValueName);
	}
}

template <class Tag, class Enum>
void CDerivedEnum<Tag, Enum>::VerifyEnumValue(jni::jint nativeValue, const char* const javaValueName)
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

template <class Tag, class Enum>
void CDerivedEnum<Tag, Enum>::VerifyEnumBasics()
{
	const TEnumClass::ordinal_type count = TEnumClass::count();
	VerifyEnumValueCount(count);
	for (TEnumClass::ordinal_type i = 0; i < count; ++i)
	{
		const TEnumClass& instance = TEnumClass::getByOrdinal(i);
		VerifyEnumOrdinal(instance.ordinal(), instance.meta().javaValueName);
	}
}

template <class Tag, class Enum>
void CDerivedEnum<Tag, Enum>::VerifyEnumOrdinal(jni::jint nativeOrdinal, const char* const javaValueName)
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

template <class Tag, class Enum>
void CDerivedEnum<Tag, Enum>::VerifyEnumValueCount(typename TEnumClass::ordinal_type nativeValueCount)
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

} // namespace jni
