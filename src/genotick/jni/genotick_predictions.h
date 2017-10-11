
#pragma once

#include <genotick/interface.h>
#include <genotick/jni/jni_helpers.h>

namespace genotick {
namespace jni {

class CGenotickPredictions : public IGenotickPredictionsDestructable
{
public:
	CGenotickPredictions(::jni::JNIEnv* pJavaEnv);
	virtual ~CGenotickPredictions();

private:
	static EGenotickPrediction GENOTICK_CALL GetElement(IGenotickPredictions* pThis, TGenotickInt32 index) {
		return static_cast<const CGenotickPredictions*>(pThis)->GetElementInternal(index);
	}
	static TGenotickSize GENOTICK_CALL GetElementCount(IGenotickPredictions* pThis) {
		return static_cast<const CGenotickPredictions*>(pThis)->GetElementCountInternal();
	}
	static void GENOTICK_CALL Release(IGenotickPredictions* pThis) {
		return static_cast<const CGenotickPredictions*>(pThis)->ReleaseInternal();
	}

	EGenotickPrediction GetElementInternal(TGenotickInt32 index) const;
	TGenotickSize GetElementCountInternal() const;
	void ReleaseInternal() const;

private:
	JNIEnv& m_javaEnv;
};

} // namespace jni
} // namespace genotick
