
#pragma once

#include <genotick/jni/wrapper/main_settings.h>
#include <genotick/jni/wrapper/data_lines.h>
#include <genotick/jni/wrapper/main_app_data.h>
#include <genotick/jni/wrapper/timepoint.h>
#include <genotick/jni/wrapper/timepoints.h>
#include <genotick/jni/wrapper/prediction.h>
#include <genotick/jni/wrapper/predictions.h>

namespace jni {
namespace genotick {

struct SMainInterfaceTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/MainInterface"; } };

class CMainInterface : public CClass<SMainInterfaceTag>
{
public:
	using TGetInterfaceVersionMethod = jni::StaticMethod<TagType, jni::jint()>;
	using TStartMethod = jni::StaticMethod<TagType, jni::jint(jni::jint /* sessionId */, jni::StringArray /* args */)>;
	using TGetSettingsMethod = jni::StaticMethod<TagType, CMainSettings::TObject(jni::jint /* sessionId */)>;
	using TGetDataMethod = jni::StaticMethod<TagType, CMainAppData::TObject(jni::jint /* sessionId */)>;
	using TGetTimePointsMethod = jni::StaticMethod<TagType, CTimePoints::TObject(jni::jint /* sessionId */)>;
	using TGetPredictionsMethod = jni::StaticMethod<TagType, CPredictions::TObject(jni::jint /* sessionId */, jni::String /* assetName */)>;
	using TGetNewestTimePointMethod = jni::StaticMethod<TagType, CTimePoint::TObject(jni::jint /* sessionId */)>;
	using TGetNewestPredictionsMethod = jni::StaticMethod<TagType, CPrediction::TObject(jni::jint /* sessionId */, jni::String /* assetName */)>;
	using TCreateSessionMethod = jni::StaticMethod<TagType, void(jni::jint /* sessionId */)>;
	using TClearSessionMethod = jni::StaticMethod<TagType, void(jni::jint /* sessionId */)>;
	using TClearSessionsMethod = jni::StaticMethod<TagType, void()>;

#define GENOTICK_CLASS_STATIC_METHODS(f) \
	f(TGetInterfaceVersionMethod  , getInterfaceVersion) \
	f(TStartMethod                , start              ) \
	f(TGetSettingsMethod          , getSettings        ) \
	f(TGetDataMethod              , getData            ) \
	f(TGetTimePointsMethod        , getTimePoints      ) \
	f(TGetPredictionsMethod       , getPredictions     ) \
	f(TGetNewestTimePointMethod   , getNewestTimePoint ) \
	f(TGetNewestPredictionsMethod , getNewestPrediction) \
	f(TCreateSessionMethod        , createSession      ) \
	f(TClearSessionMethod         , clearSession       ) \
	f(TClearSessionsMethod        , clearSessions      ) \

	explicit CMainInterface(jni::JNIEnv* pJavaEnv)
		: CClass<TagType>(pJavaEnv)
		GENOTICK_CLASS_STATIC_METHODS(GENOTICK_UNROLL_STATIC_METHOD_INITIALIZERS)
	{}

	jni::jint getInterfaceVersion() const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_getInterfaceVersion);
	}

	jni::jint start(const jni::jint sessionId, const jni::StringArray& array) const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_start, sessionId, array);
	}

	CMainSettings::TObject getSettings(const jni::jint sessionId) const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_getSettings, sessionId);
	}

	CMainAppData::TObject getData(const jni::jint sessionId) const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_getData, sessionId);
	}

	CTimePoints::TObject getTimePoints(const jni::jint sessionId) const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_getTimePoints, sessionId);
	}

	CPredictions::TObject getPredictions(const jni::jint sessionId, const jni::String assetName) const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_getPredictions, sessionId, assetName);
	}

	CTimePoint::TObject getNewestTimePoint(const jni::jint sessionId) const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_getNewestTimePoint, sessionId);
	}

	CPrediction::TObject getNewestPrediction(const jni::jint sessionId, const jni::String assetName) const
	{
		return GetUniqueClass()->Call(GetJavaEnv(), m_getNewestPrediction, sessionId, assetName);
	}

	void createSession(const jni::jint sessionId) const
	{
		GetUniqueClass()->Call(GetJavaEnv(), m_createSession, sessionId);
	}

	void clearSession(const jni::jint sessionId) const
	{
		GetUniqueClass()->Call(GetJavaEnv(), m_clearSession, sessionId);
	}

	void clearSessions() const
	{
		GetUniqueClass()->Call(GetJavaEnv(), m_clearSessions);
	}

private:
	GENOTICK_CLASS_STATIC_METHODS(GENOTICK_UNROLL_MEMBER_DECLARATIONS)
};

#undef GENOTICK_CLASS_STATIC_METHODS

} // namespace genotick
} // namespace jni
