
#pragma once

#include <genotick/jni/simple_remote_container.h>
#include <genotick/jni/remote/predictions.h>
#include <common/utils.h>

namespace genotick {
namespace jni {

class CGenotickPredictions
	: public IGenotickPredictions
	, public CSimpleRemoteContainer<EGenotickPrediction, remote::CPredictions>
{
private:
	using TRemoteValue = typename TRemoteElement::TGetValueMethod::ReturnType;

public:
	CGenotickPredictions(
		const TRemoteContainerObject& remoteContainerObject,
		const TRemoteContainer& remoteContainer,
		const TRemoteElement& remoteElement)
		: CSimpleRemoteContainer<TNativeElement, TRemoteContainer>(
			remoteContainerObject,
			remoteContainer,
			[&remoteElement](TNativeElement& nativeElement, const TRemoteElementObject& remoteElementObject)
				{ nativeElement = TNativeElement::get_by_value(remoteElement.GetEnumValue(remoteElementObject)); })
	{
		SGenotickPredictionsFunctions& mutableFunctions = const_cast<SGenotickPredictionsFunctions&>(functions);
		memset(&mutableFunctions, 0, sizeof(mutableFunctions));
		mutableFunctions.GetElement = GetElement;
		mutableFunctions.GetElementCount = GetElementCount;
		mutableFunctions.Release = Release;

		::utils::VerifyFunctionsStruct(functions);
		::utils::VerifyEqualPointers(&functions, static_cast<IGenotickPredictions*>(this));
	}

private:
	virtual ~CGenotickPredictions() {}

	UTILS_DELETE_COPY_CONSTRUCTOR(CGenotickPredictions)

	static const EGenotickPrediction* GENOTICK_CALL GetElement(IGenotickPredictions* pThis, TGenotickSize index) {
		return static_cast<const CGenotickPredictions*>(pThis)->GetElementPtrInternal(index);
	}
	static TGenotickSize GENOTICK_CALL GetElementCount(IGenotickPredictions* pThis) {
		return static_cast<const CGenotickPredictions*>(pThis)->GetElementCountInternal();
	}
	static void GENOTICK_CALL Release(IGenotickPredictions* pThis) {
		return static_cast<const CGenotickPredictions*>(pThis)->ReleaseInternal();
	}
};

} // namespace jni
} // namespace genotick
