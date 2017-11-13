
#pragma once

#include <genotick/jni/genotick_tpl.h>
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
#include <mutex>
#include <unordered_map>

namespace genotick {
namespace jni {

class CGenotick : public IGenotick
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

	using TThreadId      = ::std::thread::id;
	using TThreadDataMap = ::std::unordered_map<TThreadId, SThreadData>;
	using TMutex         = ::std::mutex;
	using TLockGuard     = ::std::lock_guard<TMutex>;
	
public:
	CGenotick(CLoader& loader, JavaVM& javaVM, JNIEnv& javaEnv);
	~CGenotick();

	inline bool Contains(JavaVM& javaVM) const { return (&m_javaVM == &javaVM); }

protected:
	TGenotickInt32 GetInterfaceVersion() const;
	EGenotickResult CreateSession(TGenotickSessionId sessionId) const;
	EGenotickResult RemoveSession(TGenotickSessionId sessionId) const;
	EGenotickResult RemoveAllSessions() const;
	EGenotickResult GetSettings(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const;
	EGenotickResult ChangeSettings(TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) const;
	EGenotickResult SetAssetData(TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) const;
	EGenotickResult Start(TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) const;
	EGenotickResult GetTimePoints(TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) const;
	EGenotickResult GetPredictions(TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) const;
	EGenotickResult GetNewestTimePoint(TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) const;
	EGenotickResult GetNewestPrediction(TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) const;
	EGenotickResult AttachCurrentThread(TGenotickBoolean asDaemon);
	EGenotickResult DetachCurrentThread();
	EGenotickResult Release() const;

private:
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

	CLoaderFriend& m_loader;
	JavaVM& m_javaVM;
	mutable TMutex m_mutex;
	TThreadDataMap m_threadDataMap;
};

using TGenotick = CGenotickTpl<CGenotick>;

} // namespace jni
} // namespace genotick
