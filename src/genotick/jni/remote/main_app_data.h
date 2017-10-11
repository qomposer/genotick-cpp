
#pragma once

#include <genotick/jni/remote/data_lines.h>

namespace genotick {
namespace jni {
namespace remote {

struct SMainAppDataTag { static constexpr auto Name() { return "com/alphatica/genotick/data/MainAppData"; } };

class CMainAppData : public CClass<SMainAppDataTag>
{
public:
	using TPutMethod = ::jni::Method<TagType, void(::jni::String /* assetName */, CDataLines::TObject)>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TPutMethod, put) \

	explicit CMainAppData(::jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
	}

	void put(const TObject& object, const ::jni::String& name, const CDataLines::TObject& dataLines) const
	{
		object.Call(GetJavaEnv(), m_put, name, dataLines);
	}

private:
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace remote
} // namespace jni
} // namespace genotick
