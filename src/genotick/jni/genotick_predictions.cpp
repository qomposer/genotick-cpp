
#include "genotick_predictions.h"
#include <utils.h>

namespace genotick {
namespace jni {

CGenotickPredictions::CGenotickPredictions(::jni::JNIEnv* pJavaEnv)
	: m_javaEnv(*pJavaEnv)
{
	SGenotickPredictionsFunctions& mutableFunctions = const_cast<SGenotickPredictionsFunctions&>(functions);
	mutableFunctions.GetElement = GetElement;
	mutableFunctions.GetElementCount = GetElementCount;
	mutableFunctions.Release = Release;

	utils::VerifyFunctionsStruct(functions);
}

CGenotickPredictions::~CGenotickPredictions()
{
}

EGenotickPrediction CGenotickPredictions::GetElementInternal(TGenotickInt32 index) const
{
	return EGenotickPrediction::Out;
}

TGenotickSize CGenotickPredictions::GetElementCountInternal() const
{
	return 0;
}

void CGenotickPredictions::ReleaseInternal() const
{

}

} // namespace jni
} // namespace genotick
