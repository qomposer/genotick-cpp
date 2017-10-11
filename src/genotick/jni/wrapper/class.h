
#pragma once

#include <genotick/jni/jni_helpers.h>

namespace genotick {
namespace jni {
namespace wrapper {

template <class Tag>
class CClass
{
public:
	using TagType = Tag;
	using TClass = ::jni::Class<TagType>;
	using TUniqueClass = ::jni::UniqueClass<TagType>;
	using TObject = ::jni::Object<TagType>;
	using TObjectArray = ::jni::Array<TObject>;

	explicit CClass(::jni::JNIEnv* pJavaEnv)
		: m_javaEnv(*pJavaEnv)
		, m_uniqueClass(TClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
	{}

protected:
	inline ::jni::JNIEnv& GetJavaEnv() const { return m_javaEnv; }
	inline const TUniqueClass& GetUniqueClass() const { return m_uniqueClass; }

private:
	::jni::JNIEnv& m_javaEnv;
	TUniqueClass m_uniqueClass;
};

} // namespace wrapper
} // namespace jni
} // namespace genotick
