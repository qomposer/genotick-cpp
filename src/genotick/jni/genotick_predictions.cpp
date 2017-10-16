
#include "genotick_predictions.h"
#include <utils.h>

namespace genotick {
namespace jni {

CGenotickPredictions::CGenotickPredictions(
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
	mutableFunctions.GetElement = GetElement;
	mutableFunctions.GetElementCount = GetElementCount;
	mutableFunctions.Release = Release;

	::utils::VerifyFunctionsStruct(functions);
	::utils::VerifyEqualPointers(&functions, static_cast<IGenotickPredictions*>(this));
}

} // namespace jni
} // namespace genotick
