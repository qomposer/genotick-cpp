
#pragma once

#include <genotick/jni/remote/prediction.h>

namespace genotick {
namespace jni {
namespace remote {

struct SPredictionsTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/Predictions"; } };

class CPredictions : public CClass<SPredictionsTag>
{
public:
	using TElement = CPrediction;
	using TElementObject = TElement::TObject;
	using TGetMethod = ::jni::Method<TagType, TElementObject(::jni::jint /* index */)>;
	using TSizeMethod = ::jni::Method<TagType, ::jni::jint()>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TGetMethod, get) \
	f(TSizeMethod, size) \

	explicit CPredictions(::jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
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
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace remote
} // namespace jni
} // namespace genotick
