
#pragma once

#include <cassert>
#include <cstdint>

namespace utils
{

template <class T>
inline void VerifyFunctionsStruct(const T& instance)
{
	(void)instance;
#ifdef _DEBUG
	const size_t size = sizeof(T) / sizeof(::std::uintptr_t);
	for (int i = 0; i < size; ++i)
	{
		auto ptr = reinterpret_cast<const ::std::uintptr_t*>(&instance) + i;
		assert(*ptr != 0);
	}
#endif
}

template <class TypeA, class TypeB>
inline void VerifyEqualPointers(TypeA* pA, TypeB* pB)
{
	assert(reinterpret_cast<const void*>(pA) == reinterpret_cast<const void*>(pB));
}

}
