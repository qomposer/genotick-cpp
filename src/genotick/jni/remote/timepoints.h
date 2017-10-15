
#pragma once

#include <genotick/jni/remote/timepoint.h>

namespace genotick {
namespace jni {
namespace remote {

struct STimePointsTag { static constexpr auto Name() { return "com/alphatica/genotick/timepoint/TimePoints"; } };

class CTimePoints : public CClass<STimePointsTag>
{
public:
	using TElement = CTimePoint;
	using TElementObject = TElement::TObject;
	using TGetMethod = ::jni::Method<TagType, TElementObject(::jni::jint /* index */)>;
	using TSizeMethod = ::jni::Method<TagType, ::jni::jint()>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TGetMethod, get) \
	f(TSizeMethod, size) \

	explicit CTimePoints(::jni::JNIEnv& javaEnv)
		: CClass<TagType>(javaEnv)
	{}

	TElementObject get(const TObject& object, const ::jni::jint index) const
	{
		return object.Call(GetJavaEnv(), m_get, index);
	}

	::jni::jint size(const TObject& object) const
	{
		return object.Call(GetJavaEnv(), m_size);
	}

private:
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_MEMBERS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace remote
} // namespace jni
} // namespace genotick
