
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
	using TSimpleContainer = CSimpleContainer<IGenotick*>;

public:
	CGenotickList(TGenotickSize size)
		: TSimpleContainer(size)
	{
		auto& mutableFunctions = const_cast<TInterfaceFunctions&>(m_functions);
		util::nullify_object_debug(mutableFunctions);

		mutableFunctions.GetElement = GetElement;
		mutableFunctions.GetElementCount = GetElementCount;
		mutableFunctions.Release = Release;

		util::verify_initialized_pointers_debug(m_functions);
		util::verify_equal_pointers_debug(&m_functions, static_cast<IGenotickList*>(this));
	}

	inline void Set(TGenotickSize index, const TElement& element)
	{
		TSimpleContainer::Set(index, element);
	}

private:
	virtual ~CGenotickList() {}

	UTILS_DELETE_COPY_CONSTRUCTOR(CGenotickList)

	static IGenotick* GENOTICK_CALL GetElement(IGenotickList* pThis, TGenotickSize index) {
		return static_cast<const CGenotickList*>(pThis)->GetElementInternal(index);
	}
	static TGenotickSize GENOTICK_CALL GetElementCount(IGenotickList* pThis) {
		return static_cast<const CGenotickList*>(pThis)->GetElementCountInternal();
	}
	static void GENOTICK_CALL Release(IGenotickList* pThis) {
		return static_cast<const CGenotickList*>(pThis)->ReleaseInternal();
	}
};

} // namespace jni
} // namespace genotick
