
#pragma once

#include <genotick/jni/simple_container.h>

namespace genotick {
namespace jni {

template <class NativeElement, class RemoteContainer>
class CSimpleRemoteContainer : public CSimpleContainer<NativeElement>
{
public:
	using TNativeContainer = ::std::vector<NativeElement>;
	using TNativeElement = NativeElement;
	using TRemoteContainer = RemoteContainer;
	using TRemoteElement = typename RemoteContainer::TElement;
	using TRemoteContainerObject = typename RemoteContainer::TObject;
	using TRemoteElementObject = typename RemoteContainer::TElementObject;

protected:
	virtual ~CSimpleRemoteContainer() {}

	template <typename TAssignPred>
	CSimpleRemoteContainer(
		const TRemoteContainerObject& remoteContainerObject,
		const TRemoteContainer& remoteContainer,
		const TAssignPred& AssignPred)
	{
		const TGenotickSize size = static_cast<TGenotickSize>(remoteContainer.size(remoteContainerObject));
		Resize(size);

		for (TGenotickSize index = 0; index < size; ++index)
		{
			const TRemoteElementObject remoteElementObject = remoteContainer.get(remoteContainerObject, index);
			Set(index, remoteElementObject, AssignPred);
		}
	}
};

} // namespace jni
} // namespace genotick
