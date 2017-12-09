
#pragma once

#include <genotick/jni/array_functions.h>
#include <common/mem.h>
#include <utility>

namespace genotick {
namespace jni {

struct empty_t {};

template <class Interface, class Elements, class Buffers = empty_t>
class CArrayBuf : public Interface
{
protected:
	using TInterface = Interface;

public:
	using TElements  = Elements;
	using TBuffers   = Buffers;
	using TElement   = typename TElements::value_type;

protected:
	CArrayBuf(TElements&& elements)
		: m_elements(std::move(elements))
	{}

	CArrayBuf(TElements&& elements, TBuffers&& buffers)
		: m_elements(std::move(elements))
		, m_buffers(std::move(buffers))
	{}

	template <typename Element = TElement>
	typename std::enable_if<!std::is_pointer<Element>::value, const TElement*>::type
		Get(TGenotickSize index) const
	{
		return &m_elements.at(index);
	}

	template <typename Element = TElement>
	typename std::enable_if<std::is_pointer<Element>::value, TElement>::type
		Get(TGenotickSize index) const
	{
		return m_elements.at(index);
	}

	TGenotickSize Size() const
	{
		return m_elements.size();
	}

	void Release()
	{
		delete this;
	}

private:
	const TElements m_elements;
	const TBuffers  m_buffers;
};

} // namespace jni
} // namespace genotick
