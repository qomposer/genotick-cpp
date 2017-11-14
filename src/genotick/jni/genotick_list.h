
#pragma once

#include <genotick/jni/simple_container.h>
#include <vector>
#include <memory>
#include <common/utils.h>

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
		SGenotickListFunctions& mutableFunctions = const_cast<SGenotickListFunctions&>(functions);
		memset(&mutableFunctions, 0, sizeof(mutableFunctions));
		mutableFunctions.GetElement = GetElement;
		mutableFunctions.GetElementCount = GetElementCount;
		mutableFunctions.Release = Release;

		::utils::VerifyFunctionsStruct(functions);
		::utils::VerifyEqualPointers(&functions, static_cast<IGenotickList*>(this));
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
