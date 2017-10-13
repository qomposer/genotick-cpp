
#pragma once

#include <genotick/interface.h>
#include <cassert>
#include <vector>

namespace genotick {
namespace jni {

template <class Element>
class CSimpleContainer
{
protected:
	using TElement = Element;
	using TContainer = ::std::vector<Element>;

private:
	using TConstIterator = typename TContainer::const_iterator;
	using TDifferenceType = typename TContainer::const_iterator::difference_type;

protected:
	inline CSimpleContainer() {}
	inline CSimpleContainer(TGenotickSize size) : m_container(static_cast<size_t>(size)) {}

	virtual ~CSimpleContainer() {}

	inline void Resize(TGenotickSize size)
	{
		m_container.resize(size);
	}

	inline void Set(TGenotickSize index, const TElement& element)
	{
		m_container[index] = element;
	}

	template <class TForeignElement, class TAssignPred>
	inline void Set(TGenotickSize index, const TForeignElement& element, const TAssignPred& AssignPred)
	{
		AssignPred(m_container[index], element);
	}

	inline TGenotickBoolean FindIndexInternal(TElement element, TGenotickSize* pIndex) const
	{
		assert(pIndex != nullptr);
		const TConstIterator first = m_container.cbegin();
		const TConstIterator last = m_container.cend();
		const TConstIterator it = ::stl::binary_find(first, last, element);
		if (it != last)
		{
			const TDifferenceType index = ::std::distance(first, it);
			*pIndex = static_cast<TGenotickSize>(index);
			return GenotickTrue;
		}
		return GenotickFalse;
	}

	inline TElement GetElementInternal(TGenotickSize index) const
	{
		return m_container[index];
	}

	inline TGenotickSize GetElementCountInternal() const
	{
		return static_cast<TGenotickSize>(m_container.size());
	}

	inline void ReleaseInternal() const
	{
		delete this;
	}

private:
	TContainer m_container;
};

} // namespace jni
} // namespace genotick
