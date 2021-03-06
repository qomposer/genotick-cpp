
#pragma once

#pragma warning(disable: 4018 4063 4624 4800)
#undef max
#include <jni/jni.hpp>
#pragma warning(default: 4018 4063 4624 4800)
#include <vector>

#define GENOTICK_UNROLL_FIELD_MEMBERS(type, name) \
const type m_##name = this->GetUniqueClass()->GetField<typename type::FieldType>(this->GetJavaEnv(), #name);

#define GENOTICK_UNROLL_STATIC_FIELD_MEMBERS(type, name) \
const type m_##name = this->GetUniqueClass()->GetStaticField<typename type::FieldType>(this->GetJavaEnv(), #name);

#define GENOTICK_UNROLL_METHOD_MEMBERS(type, name) \
const type m_##name = this->GetUniqueClass()->GetMethod<typename type::MethodType>(this->GetJavaEnv(), #name);

#define GENOTICK_UNROLL_STATIC_METHOD_MEMBERS(type, name) \
const type m_##name = this->GetUniqueClass()->GetStaticMethod<typename type::MethodType>(this->GetJavaEnv(), #name);

#define GENOTICK_UNROLL_SET_FIELD_INLINE_FUNCTIONS(type, name) \
inline void Set_##name(const TObject& object, const typename type::FieldType& value) const { \
	object.Set(this->GetJavaEnv(), this->m_##name, value); }

#define GENOTICK_UNROLL_GET_FIELD_INLINE_FUNCTIONS(type, name) \
inline typename type::FieldType Get_##name(const TObject& object) const { \
	return object.Get(this->GetJavaEnv(), this->m_##name); }

#define GENOTICK_UNROLL_SET_STATIC_FIELD_INLINE_FUNCTIONS(type, name) \
inline void Set_##name(const typename type::FieldType& value) const { \
	this->GetUniqueClass()->Set(this->GetJavaEnv(), this->m_##name, value); }

#define GENOTICK_UNROLL_GET_STATIC_FIELD_INLINE_FUNCTIONS(type, name) \
inline typename type::FieldType Get_##name() const { \
	return this->GetUniqueClass()->Get(this->GetJavaEnv(), this->m_##name); }

namespace jni
{
	template < class Result, class... P >
	Result MakeObjectArray(P&&... p)
	{
		return MakeObjectArrayF(ThingToMake<Result>(), std::forward<P>(p)...);
	}

	template < class T >
	inline std::vector<T> MakeObjectArrayF(ThingToMake<std::vector<T>>, JNIEnv& env, const Array<T>& array)
	{
		NullCheck(env, array.Get());
		const jsize length = GetArrayLength(env, *array);
		std::vector<T> result(length);
		result.resize(length);
		for (jsize i = 0; i < length; ++i)
			result[i] = array.Get(env, i);
		return result;
	}

	template < class T, class JavaClass >
	inline Array<T> MakeObjectArrayF(ThingToMake<Array<T>>, JNIEnv& env, const std::vector<T>& array, const JavaClass& clazz)
	{
		const jsize length = array.size();
		Array<T> result = Array<T>::New(env, length, clazz);
		for (jsize i = 0; i < length; ++i)
			result.Set(env, i, array[i]);
		return result;
	}

	struct ThrowableTag { static constexpr auto Name() { return "java/lang/Throwable"; } };
	struct ClassTag { static constexpr auto Name() { return "java/lang/Class"; } };
	template <> struct UntaggedObjectType<ThrowableTag> { using Type = ::jni::jthrowable; };
	template <> struct UntaggedObjectType<ClassTag> { using Type = ::jni::jclass; };
	using ThrowableObject = ::jni::Object<ThrowableTag>;
	using ClassObject = ::jni::Object<ClassTag>;

	using StringArray = ::jni::Array<::jni::String>;
	using StringClass = ::jni::Class<::jni::StringTag>;
	using UniqueStringClass = ::jni::UniqueClass<::jni::StringTag>;

	template <class ClassTag> using ObjectArray = ::jni::Array<::jni::Object<ClassTag>>;

	template <class ClassTag> using IntField = ::jni::Field<ClassTag, ::jni::jint>;
	template <class ClassTag> using LongField = ::jni::Field<ClassTag, ::jni::jlong>;
	template <class ClassTag> using ByteField = ::jni::Field<ClassTag, ::jni::jbyte>;
	template <class ClassTag> using BooleanField = ::jni::Field<ClassTag, ::jni::jboolean>;
	template <class ClassTag> using CharField = ::jni::Field<ClassTag, ::jni::jchar>;
	template <class ClassTag> using ShortField = ::jni::Field<ClassTag, ::jni::jshort>;
	template <class ClassTag> using FloatField = ::jni::Field<ClassTag, ::jni::jfloat>;
	template <class ClassTag> using DoubleField = ::jni::Field<ClassTag, ::jni::jdouble>;
	template <class ClassTag> using StringField = ::jni::Field<ClassTag, ::jni::String>;

	template <class ClassTag> using IntStaticField = ::jni::StaticField<ClassTag, ::jni::jint>;
	template <class ClassTag> using LongStaticField = ::jni::StaticField<ClassTag, ::jni::jlong>;
	template <class ClassTag> using ByteStaticField = ::jni::StaticField<ClassTag, ::jni::jbyte>;
	template <class ClassTag> using BooleanStaticField = ::jni::StaticField<ClassTag, ::jni::jboolean>;
	template <class ClassTag> using CharStaticField = ::jni::StaticField<ClassTag, ::jni::jchar>;
	template <class ClassTag> using ShortStaticField = ::jni::StaticField<ClassTag, ::jni::jshort>;
	template <class ClassTag> using FloatStaticField = ::jni::StaticField<ClassTag, ::jni::jfloat>;
	template <class ClassTag> using DoubleStaticField = ::jni::StaticField<ClassTag, ::jni::jdouble>;
	template <class ClassTag> using StringStaticField = ::jni::StaticField<ClassTag, ::jni::String>;
}
