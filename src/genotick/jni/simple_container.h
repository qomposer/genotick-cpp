
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
	using TElementPtr = const TElement*;
	using TContainer = std::vector<Element>;

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

	template <class TForeignElement, class TAssignFunc>
	inline void Set(TGenotickSize index, const TForeignElement& element, const TAssignFunc& AssignFunc)
	{
		AssignFunc(m_container[index], element);
	}

	inline TElementPtr GetPtrInternal(TGenotickSize index) const
	{
		return &m_container.at(index);
	}

	inline TElement GetInternal(TGenotickSize index) const
	{
		return m_container[index];
	}

	inline TGenotickSize SizeInternal() const
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
