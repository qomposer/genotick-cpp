
#pragma once

#include "jni_class.h"

namespace jni {
namespace genotick {

struct STimePointTag { static constexpr auto Name() { return "com/alphatica/genotick/timepoint/TimePoint"; } };

class CTimePoint : public CClass<STimePointTag>
{
public:
	using TTimePointConstructor = jni::Constructor<TagType, jni::jlong>;
	using TGetValueMethod = jni::Method<TagType, jni::jlong()>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TGetValueMethod, getValue) \

	explicit CTimePoint(jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		, m_constructor(GetUniqueClass()->GetConstructor<jni::jlong>(GetJavaEnv()))
		GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
	}

	TObject New(const jni::jlong value) const
	{
		return GetUniqueClass()->New(GetJavaEnv(), m_constructor, value);
	}

	jni::jlong getValue(const TObject& object) const
	{
		return object.Call(GetJavaEnv(), m_getValue);
	}

private:
	TTimePointConstructor m_constructor;
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace genotick
} // namespace jni
