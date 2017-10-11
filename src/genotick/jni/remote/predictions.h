
#pragma once

#include <genotick/jni/remote/prediction.h>

namespace genotick {
namespace jni {
namespace wrapper {

struct SPredictionsTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/Predictions"; } };

class CPredictions : public CClass<SPredictionsTag>
{
public:
	using TGetMethod = ::jni::Method<TagType, CPrediction::TObject(::jni::jint /* index */)>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TGetMethod, get) \

	explicit CPredictions(::jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{}

	CPrediction::TObject get(const TObject& object, const ::jni::jint index) const
	{
		return object.Call(GetJavaEnv(), m_get, index);
	}

private:
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace wrapper
} // namespace jni
} // namespace genotick
