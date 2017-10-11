
#pragma once

#include <genotick/interface.h>
#include <genotick/jni/jni_helpers.h>

namespace genotick {
namespace jni {

class CGenotickTimePoints : public IGenotickTimePointsDestructable
{
public:
	CGenotickTimePoints(::jni::JNIEnv* pJavaEnv);
	virtual ~CGenotickTimePoints();

private:
	static TGenotickBoolean GENOTICK_CALL FindIndex(IGenotickTimePoints* pThis, TGenotickTimePoint timePoint, TGenotickInt32* pIndex) {
		return static_cast<const CGenotickTimePoints*>(pThis)->FindIndexInternal(timePoint, pIndex);
	}
	static TGenotickTimePoint GENOTICK_CALL GetElement(IGenotickTimePoints* pThis, TGenotickInt32 index) {
		return static_cast<const CGenotickTimePoints*>(pThis)->GetElementInternal(index);
	}
	static TGenotickSize GENOTICK_CALL GetElementCount(IGenotickTimePoints* pThis) {
		return static_cast<const CGenotickTimePoints*>(pThis)->GetElementCountInternal();
	}
	static void GENOTICK_CALL Release(IGenotickTimePoints* pThis) {
		return static_cast<const CGenotickTimePoints*>(pThis)->ReleaseInternal();
	}

	TGenotickBoolean FindIndexInternal(TGenotickTimePoint timePoint, TGenotickInt32* pIndex) const;
	TGenotickTimePoint GetElementInternal(TGenotickInt32 index) const;
	TGenotickSize GetElementCountInternal() const;
	void ReleaseInternal() const;

private:
	JNIEnv& m_javaEnv;
};

} // namespace jni
} // namespace genotick
