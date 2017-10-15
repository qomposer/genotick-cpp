
#pragma once

#include <genotick/jni/remote/class.h>

namespace genotick {
namespace jni {
namespace remote {

struct STimePointTag { static constexpr auto Name() { return "com/alphatica/genotick/timepoint/TimePoint"; } };

class CTimePoint : public CClass<STimePointTag>
{
public:
	using TConstructor = ::jni::Constructor<TagType, ::jni::jlong>;
	using TGetValueMethod = ::jni::Method<TagType, ::jni::jlong()>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TGetValueMethod, getValue) \

	explicit CTimePoint(::jni::JNIEnv& javaEnv)
		: CClass<TagType>(javaEnv)
		, m_constructor(GetUniqueClass()->GetConstructor<::jni::jlong>(GetJavaEnv()))
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
	const TConstructor m_constructor;
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_MEMBERS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace remote
} // namespace jni
} // namespace genotick
