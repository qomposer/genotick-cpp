
#pragma once

#include <vector>
#include <utils.h>
#include <cassert>

namespace genotick {
namespace jni {

template <class NativeElement, class RemoteContainer>
class CGenotickContainer
{
public:
	using TNativeContainer = ::std::vector<NativeElement>;
	using TNativeElement = NativeElement;
	using TRemoteContainer = RemoteContainer;
	using TRemoteElement = typename RemoteContainer::TElement;
	using TRemoteContainerObject = typename RemoteContainer::TObject;
	using TRemoteElementObject = typename RemoteContainer::TElementObject;

private:
	using TConstIterator = typename TNativeContainer::const_iterator;
	using TDifferenceType = typename TNativeContainer::const_iterator::difference_type;

protected:
	virtual ~CGenotickContainer() {}

	template <typename TAssignPred>
	CGenotickContainer(
		const TRemoteContainerObject& remoteContainerObject,
		const TRemoteContainer& remoteContainer,
		const TAssignPred& AssignPred)
	{
		const size_t size = static_cast<size_t>(remoteContainer.size(remoteContainerObject));
		m_container.resize(size);
		for (size_t index = 0; index < size; ++index)
		{
			const TRemoteElementObject remoteElementObject = remoteContainer.get(remoteContainerObject, index);
			AssignPred(m_container[index], remoteElementObject);
		}
	}

	TGenotickBoolean FindIndexInternal(TNativeElement timePoint, TGenotickInt32* pIndex) const
	{
		assert(pIndex != nullptr);
		const TConstIterator first = m_container.cbegin();
		const TConstIterator last = m_container.cend();
		const TConstIterator it = ::stl::binary_find(first, last, timePoint);
		if (it != last)
		{
			const TDifferenceType index = ::std::distance(first, it);
			*pIndex = static_cast<TGenotickInt32>(index);
			return GenotickTrue;
		}
		return GenotickFalse;
	}

	TNativeElement GetElementInternal(TGenotickInt32 index) const
	{
		return m_container[index];
	}

	TGenotickSize GetElementCountInternal() const
	{
		return static_cast<TGenotickSize>(m_container.size());
	}

	void ReleaseInternal() const
	{
		delete this;
	}

private:
	TNativeContainer m_container;
};

} // namespace jni
} // namespace genotick
