
#include "genotick_timepoints.h"
#include <utils.h>

namespace genotick {
namespace jni {

CGenotickTimePoints::CGenotickTimePoints(::jni::JNIEnv* pJavaEnv)
	: m_javaEnv(*pJavaEnv)
{
	SGenotickTimePointsFunctions& mutableFunctions = const_cast<SGenotickTimePointsFunctions&>(functions);
	mutableFunctions.FindIndex = FindIndex;
	mutableFunctions.GetElement = GetElement;
	mutableFunctions.GetElementCount = GetElementCount;
	mutableFunctions.Release = Release;

	utils::VerifyFunctionsStruct(functions);
}

CGenotickTimePoints::~CGenotickTimePoints()
{
}

TGenotickBoolean CGenotickTimePoints::FindIndexInternal(TGenotickTimePoint timePoint, TGenotickInt32* pIndex) const
{
	return GenotickFalse;
}

TGenotickTimePoint CGenotickTimePoints::GetElementInternal(TGenotickInt32 index) const
{
	return 0;
}

TGenotickSize CGenotickTimePoints::GetElementCountInternal() const
{
	return 0;
}

void CGenotickTimePoints::ReleaseInternal() const
{

}

} // namespace jni
} // namespace genotick
