
#pragma once

#include <genotick/jni/simple_container.h>
#include <vector>
#include <memory>
#include <common/util.h>

namespace genotick {
namespace jni {

class CGenotickList
	: public IGenotickList
	, public CSimpleContainer<IGenotick*>
{
private:
	using TThis               = CGenotickList;
	using TInterface          = IGenotickList;
	using TInterfaceFunctions = typename TInterface::TInterfaceFunctions;
	using TSimpleContainer    = CSimpleContainer<IGenotick*>;

public:
	CGenotickList(TGenotickSize size)
		: TSimpleContainer(size)
	{
		auto& mutableFunctions = const_cast<TInterfaceFunctions&>(m_functions);
		util::nullify_object_debug(mutableFunctions);

		mutableFunctions.Get = [](const TInterface* pThis, TGenotickSize index) {
			return static_cast<const TThis*>(pThis)->GetInternal(index);
		};
		mutableFunctions.Size = [](const TInterface* pThis) {
			return static_cast<const TThis*>(pThis)->SizeInternal();
		};
		mutableFunctions.Release = [](TInterface* pThis) {
			return static_cast<const TThis*>(pThis)->ReleaseInternal();
		};

		util::verify_initialized_pointers_debug(m_functions);
		util::verify_equal_pointers_debug(&m_functions, static_cast<TInterface*>(this));
	}

	inline void Set(TGenotickSize index, const TElement& element)
	{
		TSimpleContainer::Set(index, element);
	}

private:
	virtual ~CGenotickList() {}

	UTILS_DELETE_COPY_CONSTRUCTOR(TThis)
};

} // namespace jni
} // namespace genotick
