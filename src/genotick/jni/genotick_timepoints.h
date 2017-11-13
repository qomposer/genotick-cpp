
#pragma once

#include <genotick/jni/simple_remote_container.h>
#include <genotick/jni/remote/timepoints.h>
#include <utils.h>

namespace genotick {
namespace jni {

class CGenotickTimePoints
	: public IGenotickTimePoints
	, public CSimpleRemoteContainer<TGenotickTimePoint, remote::CTimePoints>
{
private:
	using TRemoteValue = typename TRemoteElement::TGetValueMethod::ReturnType;

public:
	CGenotickTimePoints(
		const TRemoteContainerObject& remoteContainerObject,
		const TRemoteContainer& remoteContainer,
		const TRemoteElement& remoteElement)
		: CSimpleRemoteContainer<TNativeElement, TRemoteContainer>(
			remoteContainerObject,
			remoteContainer,
			[&remoteElement](TNativeElement& nativeElement, const TRemoteElementObject& remoteElementObject)
				{ nativeElement = remoteElement.getValue(remoteElementObject); })
	{
		SGenotickTimePointsFunctions& mutableFunctions = const_cast<SGenotickTimePointsFunctions&>(functions);
		memset(&mutableFunctions, 0, sizeof(mutableFunctions));
		mutableFunctions.FindIndex = FindIndex;
		mutableFunctions.GetElement = GetElement;
		mutableFunctions.GetElementCount = GetElementCount;
		mutableFunctions.Release = Release;

		::utils::VerifyFunctionsStruct(functions);
		::utils::VerifyEqualPointers(&functions, static_cast<IGenotickTimePoints*>(this));
	}

private:
	virtual ~CGenotickTimePoints() {}

	UTILS_DELETE_COPY_CONSTRUCTOR(CGenotickTimePoints)

	static TGenotickBoolean GENOTICK_CALL FindIndex(IGenotickTimePoints* pThis, const TGenotickTimePoint* timePoint, TGenotickSize* pIndex) {
		return static_cast<const CGenotickTimePoints*>(pThis)->FindIndexInternal(timePoint, pIndex);
	}
	static const TGenotickTimePoint* GENOTICK_CALL GetElement(IGenotickTimePoints* pThis, TGenotickSize index) {
		return static_cast<const CGenotickTimePoints*>(pThis)->GetElementPtrInternal(index);
	}
	static TGenotickSize GENOTICK_CALL GetElementCount(IGenotickTimePoints* pThis) {
		return static_cast<const CGenotickTimePoints*>(pThis)->GetElementCountInternal();
	}
	static void GENOTICK_CALL Release(IGenotickTimePoints* pThis) {
		return static_cast<const CGenotickTimePoints*>(pThis)->ReleaseInternal();
	}
};

} // namespace jni
} // namespace genotick
