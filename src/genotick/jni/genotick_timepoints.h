
#pragma once

#include <genotick/jni/simple_remote_container.h>
#include <genotick/jni/remote/timepoints.h>
#include <common/util.h>

namespace genotick {
namespace jni {

class CGenotickTimePoints
	: public IGenotickTimePoints
	, public CSimpleRemoteContainer<TGenotickTimePoint, remote::CTimePoints>
{
private:
	using TThis               = CGenotickTimePoints;
	using TInterface          = IGenotickTimePoints;
	using TInterfaceFunctions = typename TInterface::TInterfaceFunctions;
	using TRemoteValue        = typename TRemoteElement::TGetValueMethod::ReturnType;

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
		auto& mutableFunctions = const_cast<TInterfaceFunctions&>(m_functions);
		util::nullify_object_debug(mutableFunctions);

		mutableFunctions.FindIndex = [](const TInterface* pThis, const TGenotickTimePoint* timePoint, TGenotickSize* pIndex) {
			return static_cast<const TThis*>(pThis)->FindIndexInternal(timePoint, pIndex);
		};
		mutableFunctions.GetElement = [](const TInterface* pThis, TGenotickSize index) {
			return static_cast<const TThis*>(pThis)->GetElementPtrInternal(index);
		};
		mutableFunctions.GetElementCount = [](const TInterface* pThis) {
			return static_cast<const TThis*>(pThis)->GetElementCountInternal();
		};
		mutableFunctions.Release = [](TInterface* pThis) {
			return static_cast<const TThis*>(pThis)->ReleaseInternal();
		};

		util::verify_initialized_pointers_debug(m_functions);
		util::verify_equal_pointers_debug(&m_functions, static_cast<TInterface*>(this));
	}

private:
	virtual ~CGenotickTimePoints() {}

	UTILS_DELETE_COPY_CONSTRUCTOR(TThis)
};

} // namespace jni
} // namespace genotick
