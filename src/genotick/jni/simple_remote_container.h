
#pragma once

#include <genotick/jni/simple_container.h>

namespace genotick {
namespace jni {

template <class NativeElement, class RemoteContainer>
class CSimpleRemoteContainer : public CSimpleContainer<NativeElement>
{
protected:
	using TNativeContainer = ::std::vector<NativeElement>;
	using TNativeElement = NativeElement;
	using TRemoteContainer = RemoteContainer;
	using TRemoteElement = typename RemoteContainer::TElement;
	using TRemoteContainerObject = typename RemoteContainer::TObject;
	using TRemoteElementObject = typename RemoteContainer::TElementObject;

protected:
	template <typename TAssignFunc>
	CSimpleRemoteContainer(
		const TRemoteContainerObject& remoteContainerObject,
		const TRemoteContainer& remoteContainer,
		const TAssignFunc& AssignFunc)
	{
		const TGenotickSize size = static_cast<TGenotickSize>(remoteContainer.size(remoteContainerObject));
		Resize(size);

		for (TGenotickSize index = 0; index < size; ++index)
		{
			const TRemoteElementObject remoteElementObject = remoteContainer.get(remoteContainerObject, index);
			Set(index, remoteElementObject, AssignFunc);
		}
	}

	virtual ~CSimpleRemoteContainer() {}
};

} // namespace jni
} // namespace genotick
