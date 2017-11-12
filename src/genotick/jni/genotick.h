
#pragma once

#include <genotick/interface.h>
#include <genotick/loader.h>
#include <genotick/jni/remote/main_interface.h>
#include <genotick/jni/remote/main_settings.h>
#include <genotick/jni/remote/data_lines.h>
#include <genotick/jni/remote/main_app_data.h>
#include <genotick/jni/remote/timepoint.h>
#include <genotick/jni/remote/timepoints.h>
#include <genotick/jni/remote/weight_mode.h>
#include <genotick/jni/remote/inherited_weight_mode.h>
#include <genotick/jni/remote/chart_mode.h>
#include <genotick/jni/remote/error_code.h>
#include <genotick/jni/remote/prediction.h>
#include <genotick/jni/remote/predictions.h>
#include <genotick/jni/exceptions.h>
#include <strlcpy.h>
#include <thread>
#include <unordered_map>

namespace genotick {
namespace jni {

class CGenotick : public IGenotickDestructable
{
private:
	struct SThreadData
	{
		SThreadData(JNIEnv& javaEnv)
			: javaEnv(javaEnv)
			, remoteString(::jni::StringClass::Find(javaEnv).NewGlobalRef(javaEnv))
			, remoteMainInterface(javaEnv)
			, remoteMainSettings(javaEnv)
			, remoteDataLines(javaEnv)
			, remoteMainAppData(javaEnv)
			, remoteTimePoint(javaEnv)
			, remoteTimePoints(javaEnv)
			, remoteWeightMode(javaEnv)
			, remoteInheritedWeightMode(javaEnv)
			, remoteChartMode(javaEnv)
			, remoteErrorCode(javaEnv)
			, remotePrediction(javaEnv)
			, remotePredictions(javaEnv)
		{}

		JNIEnv& javaEnv;
		::jni::UniqueStringClass remoteString;
		remote::CMainInterface remoteMainInterface;
		remote::CMainSettings remoteMainSettings;
		remote::CDataLines remoteDataLines;
		remote::CMainAppData remoteMainAppData;
		remote::CTimePoint remoteTimePoint;
		remote::CTimePoints remoteTimePoints;
		remote::CWeightMode remoteWeightMode;
		remote::CInheritedWeightMode remoteInheritedWeightMode;
		remote::CChartMode remoteChartMode;
		remote::CErrorCode remoteErrorCode;
		remote::CPrediction remotePrediction;
		remote::CPredictions remotePredictions;
	};

	using TThreadId = ::std::thread::id;
	using TThreadDataMap = ::std::unordered_map<TThreadId, SThreadData>;
	
public:
	CGenotick(CLoader& loader, JavaVM& javaVM, JNIEnv& javaEnv);
	virtual ~CGenotick();

	CGenotick(const CGenotick&) = delete;
	CGenotick& operator=(const CGenotick&) = delete;

private:
	static TGenotickInt32 GENOTICK_CALL GetInterfaceVersion(IGenotick* pThis) {
		return static_cast<const CGenotick*>(pThis)->GetInterfaceVersionInternal();
	}
	static EGenotickResult GENOTICK_CALL CreateSession(IGenotick* pThis, TGenotickSessionId sessionId) {
		return static_cast<const CGenotick*>(pThis)->CreateSessionInternal(sessionId);
	}
	static EGenotickResult GENOTICK_CALL RemoveSession(IGenotick* pThis, TGenotickSessionId sessionId) {
		return static_cast<const CGenotick*>(pThis)->RemoveSessionInternal(sessionId);
	}
	static EGenotickResult GENOTICK_CALL RemoveAllSessions(IGenotick* pThis) {
		return static_cast<const CGenotick*>(pThis)->RemoveAllSessionsInternal();
	}
	static EGenotickResult GENOTICK_CALL GetSettings(IGenotick* pThis, TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) {
		return static_cast<const CGenotick*>(pThis)->GetSettingsInternal(sessionId, pSettings);
	}
	static EGenotickResult GENOTICK_CALL ChangeSettings(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) {
		return static_cast<const CGenotick*>(pThis)->ChangeSettingsInternal(sessionId, pSettings);
	}
	static EGenotickResult GENOTICK_CALL SetAssetData(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) {
		return static_cast<const CGenotick*>(pThis)->SetAssetDataInternal(sessionId, pAssetData);
	}
	static EGenotickResult GENOTICK_CALL Start(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) {
		return static_cast<const CGenotick*>(pThis)->StartInternal(sessionId, pArgs);
	}
	static EGenotickResult GENOTICK_CALL GetTimePoints(IGenotick* pThis, TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) {
		return static_cast<const CGenotick*>(pThis)->GetTimePointsInternal(sessionId, ppTimePoints);
	}
	static EGenotickResult GENOTICK_CALL GetPredictions(IGenotick* pThis, TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) {
		return static_cast<const CGenotick*>(pThis)->GetPredictionsInternal(sessionId, assetName, ppPredictions);
	}
	static EGenotickResult GENOTICK_CALL GetNewestTimePoint(IGenotick* pThis, TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) {
		return static_cast<const CGenotick*>(pThis)->GetNewestTimePointInternal(sessionId, pTimePoint);
	}
	static EGenotickResult GENOTICK_CALL GetNewestPrediction(IGenotick* pThis, TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) {
		return static_cast<const CGenotick*>(pThis)->GetNewestPredictionInternal(sessionId, assetName, pPrediction);
	}
	static EGenotickResult GENOTICK_CALL AttachCurrentThread(IGenotick* pThis, TGenotickBoolean asDaemon) {
		return static_cast<      CGenotick*>(pThis)->AttachCurrentThreadInternal(asDaemon);
	}
	static EGenotickResult GENOTICK_CALL DetachCurrentThread(IGenotick* pThis) {
		return static_cast<      CGenotick*>(pThis)->DetachCurrentThreadInternal();
	}
	static EGenotickResult GENOTICK_CALL Release(IGenotick* pThis) {
		return static_cast<const CGenotick*>(pThis)->ReleaseInternal();
	}

	TGenotickInt32 GetInterfaceVersionInternal() const;
	EGenotickResult CreateSessionInternal(TGenotickSessionId sessionId) const;
	EGenotickResult RemoveSessionInternal(TGenotickSessionId sessionId) const;
	EGenotickResult RemoveAllSessionsInternal() const;
	EGenotickResult GetSettingsInternal(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const;
	EGenotickResult ChangeSettingsInternal(TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) const;
	EGenotickResult SetAssetDataInternal(TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) const;
	EGenotickResult StartInternal(TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) const;
	EGenotickResult GetTimePointsInternal(TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) const;
	EGenotickResult GetPredictionsInternal(TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) const;
	EGenotickResult GetNewestTimePointInternal(TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) const;
	EGenotickResult GetNewestPredictionInternal(TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) const;
	EGenotickResult AttachCurrentThreadInternal(TGenotickBoolean asDaemon);
	EGenotickResult DetachCurrentThreadInternal();
	EGenotickResult ReleaseInternal() const;

	void AddThreadData(JNIEnv& javaEnv);
	void RemoveThreadData();
	bool HasThreadData() const;
	const SThreadData& GetThreadData() const;

	template <class D, class S> void ToNative(D& dst, const S src) const {
		dst = static_cast<D>(src);
	}

	template <> void ToNative(TGenotickString& dst, const ::jni::String src) const {
		if (dst.capacity > 0u) {
			std::string buf = ::jni::Make<std::string>(GetThreadData().javaEnv, src);
			strlcpy(dst.utf8buffer, buf.c_str(), dst.capacity);
		}
	}

	template <> void ToNative(TGenotickTimePoint& dst, const remote::CTimePoint::TObject src) const {
		dst = static_cast<TGenotickTimePoint>(GetThreadData().remoteTimePoint.getValue(src));
	}

	template <> void ToNative(EGenotickWeightMode& dst, const remote::CWeightMode::TObject src) const {
		dst = EGenotickWeightMode::get_by_value(GetThreadData().remoteWeightMode.GetEnumValue(src));
	}

	template <> void ToNative(EGenotickInheritedWeightMode& dst, const remote::CInheritedWeightMode::TObject src) const {
		dst = EGenotickInheritedWeightMode::get_by_value(GetThreadData().remoteInheritedWeightMode.GetEnumValue(src));
	}

	template <> void ToNative(EGenotickChartMode& dst, const remote::CChartMode::TObject src) const {
		dst = EGenotickChartMode::get_by_value(GetThreadData().remoteChartMode.GetEnumValue(src));
	}

	template <class D, class S> D ToJava(const S src) const {
		return static_cast<D>(src);
	}

	template <> ::jni::String ToJava(const TGenotickString src) const {
		const std::string buf = (src.utf8buffer != nullptr) ? src.utf8buffer : "";
		return ::jni::Make<::jni::String>(GetThreadData().javaEnv, buf);
	}

	template <> remote::CTimePoint::TObject ToJava(const TGenotickTimePoint src) const {
		return GetThreadData().remoteTimePoint.New(static_cast<::jni::jlong>(src));
	}

	template <> remote::CWeightMode::TObject ToJava(const EGenotickWeightMode src) const {
		return GetThreadData().remoteWeightMode.GetEnumObject(src.value());
	}

	template <> remote::CInheritedWeightMode::TObject ToJava(const EGenotickInheritedWeightMode src) const {
		return GetThreadData().remoteInheritedWeightMode.GetEnumObject(src.value());
	}

	template <> remote::CChartMode::TObject ToJava(const EGenotickChartMode src) const {
		return GetThreadData().remoteChartMode.GetEnumObject(src.value());
	}

	virtual bool Contains(JavaVM& javaVM) const override final
	{
		return (&m_javaVM == &javaVM);
	}

	CLoaderFriend& m_loader;
	JavaVM& m_javaVM;
	TThreadDataMap m_threadDataMap;

	// TODO remove mutable
};

} // namespace jni
} // namespace genotick

