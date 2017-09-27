
#pragma once

#pragma warning(disable: 4018 4063 4624 4800)
#undef max
#include "jni/jni.hpp"
#pragma warning(default: 4018 4063 4624 4800)
#include <vector>

// Because the JNI wrapper is not enough...
// Add additional JNI helper functionality here

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
	template <> struct UntaggedObjectType<ThrowableTag> { using Type = jthrowable; };
	using Throwable = Object<ThrowableTag>;
}
