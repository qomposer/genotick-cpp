
#pragma once

#include <genotick/jni/wrapper/class.h>

namespace genotick {
namespace jni {
namespace wrapper {

struct SDataLinesTag { static constexpr auto Name() { return "com/alphatica/genotick/data/DataLines"; } };

class CDataLines : public CClass<SDataLinesTag>
{
public:
	using TConstructor1 = ::jni::Constructor<TagType, ::jni::jint, ::jni::jint, ::jni::jboolean>;
	using TSetTimeMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jlong /* value */)>;
	using TSetOpenMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jdouble /* value */)>;
	using TSetHighMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jdouble /* value */)>;
	using TSetLowMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jdouble /* value */)>;
	using TSetCloseMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jdouble /* value */)>;
	using TSetVolumeMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jdouble /* value */)>;
	using TSetOtherMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jint /* column */, ::jni::jdouble /* value */)>;

#define GENOTICK_CLASS_METHODS(f) \
	f(TSetTimeMethod  , setTime  ) \
	f(TSetOpenMethod  , setOpen  ) \
	f(TSetHighMethod  , setHigh  ) \
	f(TSetLowMethod   , setLow   ) \
	f(TSetCloseMethod , setClose ) \
	f(TSetVolumeMethod, setVolume) \
	f(TSetOtherMethod , setOther ) \

	explicit CDataLines(::jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		, m_constructor1(GetUniqueClass()->GetConstructor<::jni::jint, ::jni::jint, ::jni::jboolean>(GetJavaEnv()))
		GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_INITIALIZERS)
	{
	}

	TObject New(const ::jni::jint lineCount, const ::jni::jint columnCount, const ::jni::jboolean firstLineIsNewest) const
	{
		return GetUniqueClass()->New(GetJavaEnv(), m_constructor1, lineCount, columnCount, firstLineIsNewest);
	}

	void setTime(const TObject& object, const ::jni::jint line, const ::jni::jlong value) const
	{
		object.Call(GetJavaEnv(), m_setTime, line, value);
	}

	void setOpen(const TObject& object, const ::jni::jint line, const ::jni::jdouble value) const
	{
		object.Call(GetJavaEnv(), m_setOpen, line, value);
	}

	void setHigh(const TObject& object, const ::jni::jint line, const ::jni::jdouble value) const
	{
		object.Call(GetJavaEnv(), m_setHigh, line, value);
	}

	void setLow(const TObject& object, const ::jni::jint line, const ::jni::jdouble value) const
	{
		object.Call(GetJavaEnv(), m_setLow, line, value);
	}

	void setClose(const TObject& object, const ::jni::jint line, const ::jni::jdouble value) const
	{
		object.Call(GetJavaEnv(), m_setClose, line, value);
	}

	void setVolume(const TObject& object, const ::jni::jint line, const ::jni::jdouble value) const
	{
		object.Call(GetJavaEnv(), m_setVolume, line, value);
	}

	void setOther(const TObject& object, const ::jni::jint line, const ::jni::jint column, const ::jni::jdouble value) const
	{
		object.Call(GetJavaEnv(), m_setOther, line, column, value);
	}

private:
	TConstructor1 m_constructor1;
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace wrapper
} // namespace jni
} // namespace genotick
