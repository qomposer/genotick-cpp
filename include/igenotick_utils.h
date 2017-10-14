
// Helper functions/macros for convenience

#ifndef I_GENOTICK_UTILS_H
#define I_GENOTICK_UTILS_H

#ifdef __cplusplus

#include <cassert>

template <class T>
inline void GenotickSafeRelease(T*& p) {
	if (p) {
		p->Release();
		p = nullptr;
	}
}

template<typename T, TGenotickSize size>
inline TGenotickSize GenotickArraySize(T(&)[size])
{
	return size;
}

template<int size>
inline void GenotickSetString(TGenotickString& dst, char(&src)[size])
{
	dst.utf8_buffer = src;
	dst.capacity = size;
}

inline void GenotickSetConstString(TGenotickString& dst, const char* src)
{
	dst.utf8_buffer = const_cast<char*>(src);
	dst.capacity = 0;
}

inline EGenotickResult GenotickGetOrCreate(IGenotick** ppInstance, const TGenotickCreationSettings* pSettings)
{
	if (!ppInstance || !pSettings)
	{
		return EGenotickResult::InvalidArgument;
	}
	
	IGenotickList* pList = nullptr;
	if (GenotickGetInstances(&pList, pSettings) == EGenotickResult::Success)
	{
		assert(pList != nullptr);

		if (pList->GetElementCount() > 0)
		{
			*ppInstance = pList->GetElement(0);
			pList->Release();
			return EGenotickResult::Success;
		}
		pList->Release();
	}

	return GenotickCreate(ppInstance, pSettings);
}

#else // __cplusplus

#ifdef ZORRO_LITE_C
#define GENOTICK_ZERO_MEMORY(Struct) memset((void*)&Struct, 0, sizeof(Struct))
void GenotickSetTimePoint(TGenotickTimePoint* timePoint, long lo, long hi)
{
	timePoint->lo = lo;
	timePoint->hi = hi;
}
#else
#include <assert.h>
#define GENOTICK_SAFE_RELEASE(p) if(p) { p->Release(p); p = 0; }
#define GENOTICK_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif // ZORRO_LITE_C

inline void GenotickSetString(TGenotickString* dst, char* src, unsigned int capacity)
{
	dst->utf8_buffer = src;
	dst->capacity = capacity;
}

inline void GenotickSetConstString(TGenotickString* dst, const char* src)
{
	dst->utf8_buffer = (char*)src;
	dst->capacity = 0;
}

inline EGenotickResult GenotickGetOrCreate(IGenotick** ppInstance, const TGenotickCreationSettings* pSettings)
{
	if (!ppInstance || !pSettings)
	{
		return GenotickResult_InvalidArgument;
	}

	IGenotickList* pList = 0;
	if (GenotickGetInstances(&pList, pSettings) == GenotickResult_Success)
	{
#ifndef ZORRO_LITE_C
		assert(pList != 0);
#endif
		if (pList->GetElementCount(pList) > 0)
		{
			*ppInstance = pList->GetElement(pList, 0);
			pList->Release(pList);
			return GenotickResult_Success;
		}
		pList->Release(pList);
	}

	return GenotickCreate(ppInstance, pSettings);
}

#endif // __cplusplus

#endif // I_GENOTICK_UTILS_H
