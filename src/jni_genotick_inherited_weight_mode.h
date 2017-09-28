
#pragma once

#include "jni_helpers.h"

namespace jni {
namespace genotick {

class CInheritedWeightMode
{
public:
	explicit CInheritedWeightMode(jni::JNIEnv* pJavaEnv)
		: m_javaEnv(*pJavaEnv)
		, m_uniqueClass(TClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
	{
	}

	struct TagType { static constexpr auto Name() { return "com/alphatica/genotick/breeder/InheritedWeightMode"; } };
	using TClass = jni::Class<TagType>;
	using TUniqueClass = jni::UniqueClass<TagType>;
	using TObject = jni::Object<TagType>;

private:
	jni::JNIEnv& m_javaEnv;
	TUniqueClass m_uniqueClass;
};

} // namespace genotick
} // namespace jni
