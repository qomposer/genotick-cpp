
// Helper functions/macros for convenience

#ifndef I_GENOTICK_UTILS_H
#define I_GENOTICK_UTILS_H

#ifdef __cplusplus

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

#else

#ifndef ZORRO_LITE_C
#define GENOTICK_SAFE_RELEASE(p) if(p) { p->Release(p); p = 0; }
#endif
#define GENOTICK_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

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

#ifdef ZORRO_LITE_C
inline void GenotickSetTimePoint(TGenotickTimePoint* timePoint, long lo, long hi)
{
	timePoint->lo = lo;
	timePoint->hi = hi;
}
#endif // ZORRO_LITE_C

#endif // __cplusplus

#endif // I_GENOTICK_UTILS_H
