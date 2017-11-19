
#pragma once

#include <genotick/interface.h>
#include <genotick/jni/jni_helpers.h>
#include <common/utils.h>

namespace genotick {
namespace jni {

class CLoader;
class CLoaderFriend;

template <class Implementation>
class CGenotickFunctions : public Implementation
{
private:
	using TThis               = CGenotickFunctions;
	using TImplementation     = Implementation;
	using TInterface          = typename Implementation::TInterface;
	using TInterfaceFunctions = typename TInterface::TInterfaceFunctions;

public:
	CGenotickFunctions(CLoader& loader, JavaVM& javaVM, JNIEnv& javaEnv)
		: TImplementation(loader, javaVM, javaEnv)
	{
		auto& mutableFunctions = const_cast<TInterfaceFunctions&>(m_functions);
		utils::nullify_object_debug(mutableFunctions);

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

		utils::verify_initialized_pointers_debug(m_functions);
		utils::verify_equal_pointers_debug(&m_functions, static_cast<TInterface*>(this));
	}

	virtual ~CGenotickFunctions() {}

private:
	UTILS_DELETE_COPY_CONSTRUCTOR(TThis)

private:
	static TGenotickInt32 GENOTICK_CALL Static_GetInterfaceVersion(TInterface* pThis) {
		return static_cast<const TThis*>(pThis)->GetInterfaceVersion();
	}
	static EGenotickResult GENOTICK_CALL Static_CreateSession(TInterface* pThis, TGenotickSessionId sessionId) {
		return static_cast<const TThis*>(pThis)->CreateSession(sessionId);
	}
	static EGenotickResult GENOTICK_CALL Static_RemoveSession(TInterface* pThis, TGenotickSessionId sessionId) {
		return static_cast<const TThis*>(pThis)->RemoveSession(sessionId);
	}
	static EGenotickResult GENOTICK_CALL Static_RemoveAllSessions(TInterface* pThis) {
		return static_cast<const TThis*>(pThis)->RemoveAllSessions();
	}
	static EGenotickResult GENOTICK_CALL Static_GetSettings(TInterface* pThis, TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) {
		return static_cast<const TThis*>(pThis)->GetSettings(sessionId, pSettings);
	}
	static EGenotickResult GENOTICK_CALL Static_ChangeSettings(TInterface* pThis, TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) {
		return static_cast<const TThis*>(pThis)->ChangeSettings(sessionId, pSettings);
	}
	static EGenotickResult GENOTICK_CALL Static_SetAssetData(TInterface* pThis, TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) {
		return static_cast<const TThis*>(pThis)->SetAssetData(sessionId, pAssetData);
	}
	static EGenotickResult GENOTICK_CALL Static_Start(TInterface* pThis, TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) {
		return static_cast<const TThis*>(pThis)->Start(sessionId, pArgs);
	}
	static EGenotickResult GENOTICK_CALL Static_GetTimePoints(TInterface* pThis, TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) {
		return static_cast<const TThis*>(pThis)->GetTimePoints(sessionId, ppTimePoints);
	}
	static EGenotickResult GENOTICK_CALL Static_GetPredictions(TInterface* pThis, TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) {
		return static_cast<const TThis*>(pThis)->GetPredictions(sessionId, assetName, ppPredictions);
	}
	static EGenotickResult GENOTICK_CALL Static_GetNewestTimePoint(TInterface* pThis, TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) {
		return static_cast<const TThis*>(pThis)->GetNewestTimePoint(sessionId, pTimePoint);
	}
	static EGenotickResult GENOTICK_CALL Static_GetNewestPrediction(TInterface* pThis, TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) {
		return static_cast<const TThis*>(pThis)->GetNewestPrediction(sessionId, assetName, pPrediction);
	}
	static EGenotickResult GENOTICK_CALL Static_AttachCurrentThread(TInterface* pThis, TGenotickBoolean asDaemon) {
		return static_cast<      TThis*>(pThis)->AttachCurrentThread(asDaemon);
	}
	static EGenotickResult GENOTICK_CALL Static_DetachCurrentThread(TInterface* pThis) {
		return static_cast<      TThis*>(pThis)->DetachCurrentThread();
	}
	static EGenotickResult GENOTICK_CALL Static_Release(TInterface* pThis) {
		return static_cast<const TThis*>(pThis)->Release();
	}
};

} // namespace jni
} // namespace genotick
