
#pragma once

#include <genotick/jni/simple_remote_container.h>
#include <genotick/jni/remote/predictions.h>
#include <common/util.h>

namespace genotick {
namespace jni {

class CGenotickPredictions
	: public IGenotickPredictions
	, public CSimpleRemoteContainer<CGenotickPrediction, remote::CPredictions>
{
private:
	using TThis               = CGenotickPredictions;
	using TInterface          = IGenotickPredictions;
	using TInterfaceFunctions = typename TInterface::TInterfaceFunctions;
	using TRemoteValue        = typename TRemoteElement::TGetValueMethod::ReturnType;

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
		auto& mutableFunctions = const_cast<TInterfaceFunctions&>(m_functions);
		util::nullify_object_debug(mutableFunctions);

		mutableFunctions.GetElement = [](TInterface* pThis, TGenotickSize index) -> EGenotickPrediction {
			return static_cast<const TThis*>(pThis)->GetElementInternal(index);
		};
		mutableFunctions.GetElementCount = [](TInterface* pThis) {
			return static_cast<const TThis*>(pThis)->GetElementCountInternal();
		};
		mutableFunctions.Release = [](TInterface* pThis) {
			return static_cast<const TThis*>(pThis)->ReleaseInternal();
		};

		util::verify_initialized_pointers_debug(m_functions);
		util::verify_equal_pointers_debug(&m_functions, static_cast<TInterface*>(this));
	}

private:
	virtual ~CGenotickPredictions() {}

	UTILS_DELETE_COPY_CONSTRUCTOR(TThis)
};

} // namespace jni
} // namespace genotick
