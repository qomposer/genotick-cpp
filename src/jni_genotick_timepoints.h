
#pragma once

#include "jni_genotick_timepoint.h"

namespace jni {
namespace genotick {

struct STimePointsTag { static constexpr auto Name() { return "com/alphatica/genotick/timepoint/TimePoints"; } };

class CTimePoints : public CClass<STimePointsTag>
{
public:
	using TGetMethod = jni::Method<TagType, CTimePoint::TObject(jni::jint /* index */)>;
	using TGetIndexMethod = jni::Method<TagType, jni::jint(CTimePoint::TObject)>;

#define GENOTICK_TIMEPOINTS_METHODS(f) \
	f(TGetMethod, get) \
	f(TGetIndexMethod, getIndex) \

	explicit CTimePoints(jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		GENOTICK_TIMEPOINTS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{}

	CTimePoint::TObject get(const TObject& object, const jni::jint index) const
	{
		return object.Call(GetJavaEnv(), m_get, index);
	}

	jni::jint getIndex(const TObject& object, const CTimePoint::TObject& timePoint) const
	{
		return object.Call(GetJavaEnv(), m_getIndex, timePoint);
	}

private:
	GENOTICK_TIMEPOINTS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_TIMEPOINTS_METHODS

} // namespace genotick
} // namespace jni
