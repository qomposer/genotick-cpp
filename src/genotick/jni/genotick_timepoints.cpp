
#include "genotick_timepoints.h"
#include <genotick/jni/remote/timepoint.h>
#include <genotick/jni/remote/timepoints.h>
#include <genotick/jni/remote/main_interface.h>

namespace genotick {
namespace jni {

CGenotickTimePoints::CGenotickTimePoints(
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
	mutableFunctions.FindIndex = FindIndex;
	mutableFunctions.GetElement = GetElement;
	mutableFunctions.GetElementCount = GetElementCount;
	mutableFunctions.Release = Release;

	::utils::VerifyFunctionsStruct(functions);
}

} // namespace jni
} // namespace genotick
