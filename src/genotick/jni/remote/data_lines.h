
#pragma once

#include <genotick/jni/remote/class.h>
#include <genotick/jni/exceptions.h>
#include <genotick/interface.h>

namespace genotick {
namespace jni {
namespace remote {

struct SDataLinesTag { static constexpr auto Name() { return "com/alphatica/genotick/data/DataLines"; } };

class CDataLines : public CClass<SDataLinesTag>
{
public:
	using TMinColumnCountField = ::jni::IntStaticField<TagType>;
	using TConstructor1 = ::jni::Constructor<TagType, ::jni::jint, ::jni::jint, ::jni::jboolean>;
	using TSetTimeMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jlong /* value */)>;
	using TSetOpenMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jdouble /* value */)>;
	using TSetHighMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jdouble /* value */)>;
	using TSetLowMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jdouble /* value */)>;
	using TSetCloseMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jdouble /* value */)>;
	using TSetOtherMethod = ::jni::Method<TagType, void(::jni::jint /* line */, ::jni::jint /* column */, ::jni::jdouble /* value */)>;

#define GENOTICK_STATIC_CLASS_FIELDS(f) \
	f(TMinColumnCountField, MIN_COLUMN_COUNT) \

#define GENOTICK_CLASS_METHODS(f)  \
	f(TSetTimeMethod  , setTime  ) \
	f(TSetOpenMethod  , setOpen  ) \
	f(TSetHighMethod  , setHigh  ) \
	f(TSetLowMethod   , setLow   ) \
	f(TSetCloseMethod , setClose ) \
	f(TSetOtherMethod , setOther ) \

	explicit CDataLines(::jni::JNIEnv& javaEnv)
		: CClass<TagType>(javaEnv)
		, m_constructor1(GetUniqueClass()->GetConstructor<::jni::jint, ::jni::jint, ::jni::jboolean>(GetJavaEnv()))
	{
		::jni::jint expectedMinColumnCount = Get_MIN_COLUMN_COUNT();
		::jni::jint interfaceMinColumnCount = static_cast<::jni::jint>(GenotickMinColumnCount);

		if (expectedMinColumnCount != interfaceMinColumnCount)
		{
			throw EnumMismatchException(::stl::string_format(
				"Expected min column count '%d' and interface min column count '%d' do not match",
				expectedMinColumnCount, interfaceMinColumnCount));
		}
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

	void setOther(const TObject& object, const ::jni::jint line, const ::jni::jint column, const ::jni::jdouble value) const
	{
		object.Call(GetJavaEnv(), m_setOther, line, column, value);
	}

	::jni::jint Get_MIN_COLUMN_COUNT() const
	{
		return GetUniqueClass()->Get(GetJavaEnv(), m_MIN_COLUMN_COUNT);
	}

private:
	const TConstructor1 m_constructor1;
	GENOTICK_STATIC_CLASS_FIELDS(GENOTICK_UNROLL_STATIC_FIELD_MEMBERS)
	GENOTICK_CLASS_METHODS(GENOTICK_UNROLL_METHOD_MEMBERS)
};

#undef GENOTICK_CLASS_METHODS

} // namespace remote
} // namespace jni
} // namespace genotick
