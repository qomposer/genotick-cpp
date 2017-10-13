
#pragma once

#include <genotick/jni/simple_remote_container.h>
#include <genotick/jni/remote/timepoints.h>

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
		const TRemoteElement& remoteElement);

private:
	static TGenotickBoolean GENOTICK_CALL FindIndex(IGenotickTimePoints* pThis, TGenotickTimePoint timePoint, TGenotickSize* pIndex) {
		return static_cast<const CGenotickTimePoints*>(pThis)->FindIndexInternal(timePoint, pIndex);
	}
	static TGenotickTimePoint GENOTICK_CALL GetElement(IGenotickTimePoints* pThis, TGenotickSize index) {
		return static_cast<const CGenotickTimePoints*>(pThis)->GetElementInternal(index);
	}
	static TGenotickSize GENOTICK_CALL GetElementCount(IGenotickTimePoints* pThis) {
		return static_cast<const CGenotickTimePoints*>(pThis)->GetElementCountInternal();
	}
	static void GENOTICK_CALL Release(IGenotickTimePoints* pThis) {
		return static_cast<const CGenotickTimePoints*>(pThis)->ReleaseInternal();
	}

private:
	virtual ~CGenotickTimePoints() {}
};

} // namespace jni
} // namespace genotick
