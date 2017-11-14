
#pragma once

#include <genotick/interface.h>
#include <genotick/jni/jni_helpers.h>
#include <common/utils.h>

namespace genotick {
namespace jni {

class CLoader;
class CLoaderFriend;

template <class GenotickImpl>
class CGenotickTpl : public GenotickImpl
{
public:
	CGenotickTpl(CLoader& loader, JavaVM& javaVM, JNIEnv& javaEnv)
		: GenotickImpl(loader, javaVM, javaEnv)
	{
		SGenotickFunctions& mutableFunctions = const_cast<SGenotickFunctions&>(functions);
		memset(&mutableFunctions, 0, sizeof(mutableFunctions));

		mutableFunctions.GetInterfaceVersion = Static_GetInterfaceVersion;
		mutableFunctions.CreateSession       = Static_CreateSession;
		mutableFunctions.RemoveSession       = Static_RemoveSession;
		mutableFunctions.RemoveAllSessions   = Static_RemoveAllSessions;
		mutableFunctions.GetSettings         = Static_GetSettings;
		mutableFunctions.ChangeSettings      = Static_ChangeSettings;
		mutableFunctions.SetAssetData        = Static_SetAssetData;
		mutableFunctions.Start               = Static_Start;
		mutableFunctions.GetTimePoints       = Static_GetTimePoints;
		mutableFunctions.GetPredictions      = Static_GetPredictions;
		mutableFunctions.GetNewestTimePoint  = Static_GetNewestTimePoint;
		mutableFunctions.GetNewestPrediction = Static_GetNewestPrediction;
		mutableFunctions.AttachCurrentThread = Static_AttachCurrentThread;
		mutableFunctions.DetachCurrentThread = Static_DetachCurrentThread;
		mutableFunctions.Release             = Static_Release;

		::utils::VerifyFunctionsStruct(functions);
		::utils::VerifyEqualPointers(&functions, static_cast<IGenotick*>(this));
	}

	virtual ~CGenotickTpl() {}

private:
	UTILS_DELETE_COPY_CONSTRUCTOR(CGenotickTpl)

private:
	static TGenotickInt32 GENOTICK_CALL Static_GetInterfaceVersion(IGenotick* pThis) {
		return static_cast<const CGenotickTpl*>(pThis)->GetInterfaceVersion();
	}
	static EGenotickResult GENOTICK_CALL Static_CreateSession(IGenotick* pThis, TGenotickSessionId sessionId) {
		return static_cast<const CGenotickTpl*>(pThis)->CreateSession(sessionId);
	}
	static EGenotickResult GENOTICK_CALL Static_RemoveSession(IGenotick* pThis, TGenotickSessionId sessionId) {
		return static_cast<const CGenotickTpl*>(pThis)->RemoveSession(sessionId);
	}
	static EGenotickResult GENOTICK_CALL Static_RemoveAllSessions(IGenotick* pThis) {
		return static_cast<const CGenotickTpl*>(pThis)->RemoveAllSessions();
	}
	static EGenotickResult GENOTICK_CALL Static_GetSettings(IGenotick* pThis, TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) {
		return static_cast<const CGenotickTpl*>(pThis)->GetSettings(sessionId, pSettings);
	}
	static EGenotickResult GENOTICK_CALL Static_ChangeSettings(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) {
		return static_cast<const CGenotickTpl*>(pThis)->ChangeSettings(sessionId, pSettings);
	}
	static EGenotickResult GENOTICK_CALL Static_SetAssetData(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) {
		return static_cast<const CGenotickTpl*>(pThis)->SetAssetData(sessionId, pAssetData);
	}
	static EGenotickResult GENOTICK_CALL Static_Start(IGenotick* pThis, TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) {
		return static_cast<const CGenotickTpl*>(pThis)->Start(sessionId, pArgs);
	}
	static EGenotickResult GENOTICK_CALL Static_GetTimePoints(IGenotick* pThis, TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) {
		return static_cast<const CGenotickTpl*>(pThis)->GetTimePoints(sessionId, ppTimePoints);
	}
	static EGenotickResult GENOTICK_CALL Static_GetPredictions(IGenotick* pThis, TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) {
		return static_cast<const CGenotickTpl*>(pThis)->GetPredictions(sessionId, assetName, ppPredictions);
	}
	static EGenotickResult GENOTICK_CALL Static_GetNewestTimePoint(IGenotick* pThis, TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) {
		return static_cast<const CGenotickTpl*>(pThis)->GetNewestTimePoint(sessionId, pTimePoint);
	}
	static EGenotickResult GENOTICK_CALL Static_GetNewestPrediction(IGenotick* pThis, TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) {
		return static_cast<const CGenotickTpl*>(pThis)->GetNewestPrediction(sessionId, assetName, pPrediction);
	}
	static EGenotickResult GENOTICK_CALL Static_AttachCurrentThread(IGenotick* pThis, TGenotickBoolean asDaemon) {
		return static_cast<      CGenotickTpl*>(pThis)->AttachCurrentThread(asDaemon);
	}
	static EGenotickResult GENOTICK_CALL Static_DetachCurrentThread(IGenotick* pThis) {
		return static_cast<      CGenotickTpl*>(pThis)->DetachCurrentThread();
	}
	static EGenotickResult GENOTICK_CALL Static_Release(IGenotick* pThis) {
		return static_cast<const CGenotickTpl*>(pThis)->Release();
	}
};

} // namespace jni
} // namespace genotick
