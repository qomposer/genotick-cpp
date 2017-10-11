
#pragma once

#include <genotick/jni/wrapper/timepoint.h>

namespace genotick {
namespace jni {
namespace wrapper {

struct STimePointsTag { static constexpr auto Name() { return "com/alphatica/genotick/timepoint/TimePoints"; } };

class CTimePoints : public CClass<STimePointsTag>
{
public:
	using TGetMethod = ::jni::Method<TagType, CTimePoint::TObject(::jni::jint /* index */)>;
	using TGetIndexMethod = ::jni::Method<TagType, ::jni::jint(CTimePoint::TObject)>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TGetMethod, get) \
	f(TGetIndexMethod, getIndex) \

	explicit CTimePoints(::jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{}

	CTimePoint::TObject get(const TObject& object, const ::jni::jint index) const
	{
		return object.Call(GetJavaEnv(), m_get, index);
	}

	::jni::jint getIndex(const TObject& object, const CTimePoint::TObject& timePoint) const
	{
		return object.Call(GetJavaEnv(), m_getIndex, timePoint);
	}

private:
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace wrapper
} // namespace jni
} // namespace genotick
