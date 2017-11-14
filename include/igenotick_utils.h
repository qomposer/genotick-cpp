
#ifndef I_GENOTICK_UTILS_H_
#define I_GENOTICK_UTILS_H_

#ifdef __cplusplus

#include <cassert>
#include <memory>

template <class T>
inline void GenotickSafeRelease(T*& p) {
	if (p) {
		p->Release();
		p = 0;
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
	dst.utf8buffer = src;
	dst.capacity = size;
}

inline void GenotickSetConstString(TGenotickString& dst, const char* src)
{
	dst.utf8buffer = const_cast<char*>(src);
	dst.capacity = 0;
}


inline EGenotickResult GenotickGetOrCreate(IGenotick** ppInstance, const TGenotickCreationSettings* pSettings)
{
	if (!ppInstance || !pSettings)
	{
		return EGenotickResult::InvalidArgument;
	}
	
	IGenotickList* pList = 0;
	if (GenotickGetInstances(&pList, pSettings) == EGenotickResult::Success)
	{
		assert(pList != 0);

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


class CGenotickAttachCurrentThreadRAII
{
public:
	CGenotickAttachCurrentThreadRAII()
		: m_pGenotick(nullptr) {};

	EGenotickResult AttachCurrentThread(IGenotick* pGenotick, bool asDaemon)
	{
		if (!pGenotick)
		{
			return EGenotickResult::InvalidArgument;
		}
		EGenotickResult result = pGenotick->AttachCurrentThread(asDaemon);
		if (result == EGenotickResult::Success)
		{
			m_pGenotick = pGenotick;
		}
		return result;
	}

	~CGenotickAttachCurrentThreadRAII()
	{
		if (m_pGenotick)
		{
			m_pGenotick->DetachCurrentThread();
		}
	}
private:
	CGenotickAttachCurrentThreadRAII(const CGenotickAttachCurrentThreadRAII&) {};
	CGenotickAttachCurrentThreadRAII& operator=(const CGenotickAttachCurrentThreadRAII&) {};

	IGenotick* m_pGenotick;
};


class CGenotickCreateSessionRAII
{
public:
	CGenotickCreateSessionRAII()
		: m_pGenotick(nullptr)
		, m_sessionId(0) {};

	EGenotickResult CreateSession(IGenotick* pGenotick, TGenotickSessionId sessionId)
	{
		if (!pGenotick)
		{
			return EGenotickResult::InvalidArgument;
		}
		EGenotickResult result = pGenotick->CreateSession(sessionId);
		if (result == EGenotickResult::Success)
		{
			m_pGenotick = pGenotick;
			m_sessionId = sessionId;
		}
		return result;
	}

	~CGenotickCreateSessionRAII()
	{
		if (m_pGenotick)
		{
			m_pGenotick->RemoveSession(m_sessionId);
		}
	}
private:
	CGenotickCreateSessionRAII(const CGenotickCreateSessionRAII&) {};
	CGenotickCreateSessionRAII& operator=(const CGenotickCreateSessionRAII&) {};

	IGenotick* m_pGenotick;
	TGenotickSessionId m_sessionId;
};

template <class T>
class CGenotickObjectDeleter
{
public:
	void operator()(T* p) const
	{
		if (p)
		{
			p->Release();
		}
	}
};

template <class T>
using TGenotickUniquePtr = ::std::unique_ptr<T, CGenotickObjectDeleter<T>>;

using IGenotickPtr = TGenotickUniquePtr<IGenotick>;
using IGenotickListPtr = TGenotickUniquePtr<IGenotickList>;
using IGenotickPredictionsPtr = TGenotickUniquePtr<IGenotickPredictions>;
using IGenotickTimePointsPtr = TGenotickUniquePtr<IGenotickTimePoints>;

inline EGenotickResult GenotickCreate(IGenotickPtr& out, const TGenotickCreationSettings& settings)
{
	IGenotick* pRaw = nullptr;
	EGenotickResult result = GenotickCreate(&pRaw, &settings);
	out.reset(pRaw);
	return result;
}

inline EGenotickResult GenotickGetOrCreate(IGenotickPtr& out, const TGenotickCreationSettings& settings)
{
	IGenotick* pRaw = nullptr;
	EGenotickResult result = GenotickGetOrCreate(&pRaw, &settings);
	out.reset(pRaw);
	return result;
}

inline EGenotickResult GenotickGetInstances(IGenotickListPtr& out, const TGenotickCreationSettings* pSettings)
{
	IGenotickList* pRaw = nullptr;
	EGenotickResult result = GenotickGetInstances(&pRaw, pSettings);
	out.reset(pRaw);
	return result;
}

inline EGenotickResult GenotickGetTimePoints(IGenotick& genotick, TGenotickSessionId sessionId, IGenotickTimePointsPtr& out)
{
	IGenotickTimePoints* pRaw = nullptr;
	EGenotickResult result = genotick.GetTimePoints(sessionId, &pRaw);
	out.reset(pRaw);
	return result;
}

inline EGenotickResult GenotickGetPredictions(IGenotick& genotick, TGenotickSessionId sessionId, const char* assetName, IGenotickPredictionsPtr& out)
{
	IGenotickPredictions* pRaw = nullptr;
	EGenotickResult result = genotick.GetPredictions(sessionId, assetName, &pRaw);
	out.reset(pRaw);
	return result;
}

#else // __cplusplus

#ifdef ZORRO_LITE_C
#define GENOTICK_ZERO_STRUCT(Struct) memset(&Struct, 0, sizeof(Struct))
#define GENOTICK_ZERO_ARRAY(Array, Size, Type) memset(Array, 0, sizeof(Type)*Size)
void GenotickSetInt64(TGenotickInt64* pValue, long hi, long lo)
{
	pValue->hi = hi;
	pValue->lo = lo;
}
#else
#include <assert.h>
#define GENOTICK_SAFE_RELEASE(p) if(p) { p->Release(p); p = 0; }
#define GENOTICK_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif // ZORRO_LITE_C

inline void GenotickSetString(TGenotickString* dst, char* src, unsigned int capacity)
{
	dst->utf8buffer = src;
	dst->capacity = capacity;
}

inline void GenotickSetConstString(TGenotickString* dst, const char* src)
{
	dst->utf8buffer = (char*)src;
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

#endif // I_GENOTICK_UTILS_H_
