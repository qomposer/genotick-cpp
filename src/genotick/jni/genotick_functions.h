
#pragma once

#include <genotick/interface.h>
#include <genotick/jni/jni_helpers.h>
#include <common/util.h>
#include <utility>

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
	template <class... Args>
	CGenotickFunctions(Args&&... args)
		: TImplementation(std::forward<Args>(args)...)
	{
		auto& mutableFunctions = const_cast<TInterfaceFunctions&>(m_functions);
		util::nullify_object_debug(mutableFunctions);

		mutableFunctions.GetInterfaceVersion = [](const TInterface* pThis) {
			return static_cast<const TThis*>(pThis)->GetInterfaceVersion();
		};
		mutableFunctions.CreateSession = [](TInterface* pThis, TGenotickSessionId sessionId) {
			return static_cast<const TThis*>(pThis)->CreateSession(sessionId);
		};
		mutableFunctions.RemoveSession = [](TInterface* pThis, TGenotickSessionId sessionId) {
			return static_cast<const TThis*>(pThis)->RemoveSession(sessionId);
		};
		mutableFunctions.RemoveAllSessions = [](TInterface* pThis) {
			return static_cast<const TThis*>(pThis)->RemoveAllSessions();
		};
		mutableFunctions.GetSettings = [](const TInterface* pThis, TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) {
			return static_cast<const TThis*>(pThis)->GetSettings(sessionId, pSettings);
		};
		mutableFunctions.ChangeSettings = [](TInterface* pThis, TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) {
			return static_cast<const TThis*>(pThis)->ChangeSettings(sessionId, pSettings);
		};
		mutableFunctions.SetAssetData = [](TInterface* pThis, TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) {
			return static_cast<const TThis*>(pThis)->SetAssetData(sessionId, pAssetData);
		};
		mutableFunctions.Start = [](TInterface* pThis, TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) {
			return static_cast<const TThis*>(pThis)->Start(sessionId, pArgs);
		};
		mutableFunctions.GetTimePoints = [](const TInterface* pThis, TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) {
			return static_cast<const TThis*>(pThis)->GetTimePoints(sessionId, ppTimePoints);
		};
		mutableFunctions.GetPredictions = [](const TInterface* pThis, TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) {
			return static_cast<const TThis*>(pThis)->GetPredictions(sessionId, assetName, ppPredictions);
		};
		mutableFunctions.GetNewestTimePoint  = [](const TInterface* pThis, TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) {
			return static_cast<const TThis*>(pThis)->GetNewestTimePoint(sessionId, pTimePoint);
		};
		mutableFunctions.GetNewestPrediction = [](const TInterface* pThis, TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) {
			return static_cast<const TThis*>(pThis)->GetNewestPrediction(sessionId, assetName, pPrediction);
		};
		mutableFunctions.AttachCurrentThread = [](TInterface* pThis, TGenotickBoolean asDaemon) {
			return static_cast<TThis*>(pThis)->AttachCurrentThread(asDaemon);
		};
		mutableFunctions.DetachCurrentThread = [](TInterface* pThis) {
			return static_cast<TThis*>(pThis)->DetachCurrentThread();
		};
		mutableFunctions.Release = [](TInterface* pThis) {
			return static_cast<const TThis*>(pThis)->Release();
		};

		util::verify_initialized_pointers_debug(m_functions);
		util::verify_equal_pointers_debug(&m_functions, static_cast<TInterface*>(this));
	}

	virtual ~CGenotickFunctions() {}

private:
	UTILS_DELETE_COPY_CONSTRUCTOR(TThis)
};

} // namespace jni
} // namespace genotick
