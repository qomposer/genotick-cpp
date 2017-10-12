
#pragma once

#include <genotick/interface.h>
#include <genotick/jni/genotick_container.h>
#include <genotick/jni/remote/predictions.h>

namespace genotick {
namespace jni {

class CGenotickPredictions
	: public IGenotickPredictions
	, public CGenotickContainer<EGenotickPrediction, remote::CPredictions>
{
private:
	using TRemoteValue = typename TRemoteElement::TGetValueMethod::ReturnType;

public:
	CGenotickPredictions(
		const TRemoteContainerObject& remoteContainerObject,
		const TRemoteContainer& remoteContainer,
		const TRemoteElement& remoteElement);

private:
	static EGenotickPrediction GENOTICK_CALL GetElement(IGenotickPredictions* pThis, TGenotickInt32 index) {
		return static_cast<const CGenotickPredictions*>(pThis)->GetElementInternal(index);
	}
	static TGenotickSize GENOTICK_CALL GetElementCount(IGenotickPredictions* pThis) {
		return static_cast<const CGenotickPredictions*>(pThis)->GetElementCountInternal();
	}
	static void GENOTICK_CALL Release(IGenotickPredictions* pThis) {
		return static_cast<const CGenotickPredictions*>(pThis)->ReleaseInternal();
	}

private:
	virtual ~CGenotickPredictions() {}
};

} // namespace jni
} // namespace genotick
