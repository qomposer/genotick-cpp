
#pragma once

#include <igenotick.h>
#include "igenotick_destructable.h"
#include "jni_genotick_interface.h"
#include "jni_genotick_settings.h"

class CJavaLoaderGenotick;
class CJavaLoader;

namespace jni {
namespace genotick {

class CGenotick : public IGenotickDestructable
{
public:
	// Constructor can throw jni::PendingJavaException
	CGenotick(CJavaLoader* pJavaLoader, jni::JavaVM* pJavaVM, jni::JNIEnv* pJavaEnv);
	virtual ~CGenotick();

private:
	static TGenotickInt32 GENOTICK_CALL GetInterfaceVersionThis(IGenotick* pThis) {
		return static_cast<CGenotick*>(pThis)->GetInterfaceVersionInternal();
	};
	static EGenotickResult GENOTICK_CALL GetSettingsThis(IGenotick* pThis, SGenotickMainSettings* pSettings) {
		return static_cast<CGenotick*>(pThis)->GetSettingsInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL ChangeSettingsThis(IGenotick* pThis, const SGenotickMainSettings* pSettings) {
		return static_cast<CGenotick*>(pThis)->ChangeSettingsInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL StartThis(IGenotick* pThis, const SGenotickStartSettings* pSettings) {
		return static_cast<CGenotick*>(pThis)->StartInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL ReleaseThis(IGenotick* pThis) {
		return static_cast<CGenotick*>(pThis)->ReleaseInternal();
	};
	
	TGenotickInt32 GetInterfaceVersionInternal();
	EGenotickResult GetSettingsInternal(SGenotickMainSettings* pSettings);
	EGenotickResult ChangeSettingsInternal(const SGenotickMainSettings* pSettings);
	EGenotickResult StartInternal(const SGenotickStartSettings* pSettings);
	EGenotickResult ReleaseInternal();

	EGenotickResult HandleJavaException();

	CJavaLoaderGenotick& m_javaLoader;
	JavaVM& m_javaVM;
	JNIEnv& m_javaEnv;
	
	jni::UniqueStringClass m_stringClass;
	jni::genotick::CMainInterface m_mainInterface;
	jni::genotick::CMainSettings m_mainSettings;
};

} // namespace genotick
} // namespace jni
