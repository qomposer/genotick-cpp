
#pragma once

#include <genotick/jni/wrapper/class.h>

namespace genotick {
namespace jni {
namespace wrapper {

struct STimePointTag { static constexpr auto Name() { return "com/alphatica/genotick/timepoint/TimePoint"; } };

class CTimePoint : public CClass<STimePointTag>
{
public:
	using TConstructor = ::jni::Constructor<TagType, ::jni::jlong>;
	using TGetValueMethod = ::jni::Method<TagType, ::jni::jlong()>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TGetValueMethod, getValue) \

	explicit CTimePoint(::jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		, m_constructor(GetUniqueClass()->GetConstructor<::jni::jlong>(GetJavaEnv()))
		GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
	}

	TObject New(const ::jni::jlong value) const
	{
		return GetUniqueClass()->New(GetJavaEnv(), m_constructor, value);
	}

	::jni::jlong getValue(const TObject& object) const
	{
		return object.Call(GetJavaEnv(), m_getValue);
	}

private:
	TConstructor m_constructor;
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace wrapper
} // namespace jni
} // namespace genotick
