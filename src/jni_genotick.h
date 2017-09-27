
#pragma once

#include <igenotick.h>
#include "igenotick_destructable.h"
#include "jni_helpers.h"

class CJavaLoaderGenotick;
class CJavaLoader;

class CJniGenotick : public IGenotickDestructable
{
public:
	// Constructor can throw jni::PendingJavaException
	CJniGenotick(CJavaLoader* pJavaLoader, JavaVM* pJavaVM, JNIEnv* pJavaEnv);
	virtual ~CJniGenotick();

private:
	static jint_t GENOTICK_CALL GetInterfaceVersionThis(IGenotick* pThis) {
		return static_cast<CJniGenotick*>(pThis)->GetInterfaceVersionInternal();
	};
	static EGenotickResult GENOTICK_CALL StartThis(IGenotick* pThis, const SGenotickStartSettings* pSettings) {
		return static_cast<CJniGenotick*>(pThis)->StartInternal(pSettings);
	};
	static EGenotickResult GENOTICK_CALL ReleaseThis(IGenotick* pThis) {
		return static_cast<CJniGenotick*>(pThis)->ReleaseInternal();
	};
	
	jint_t GetInterfaceVersionInternal() const;
	EGenotickResult StartInternal(const SGenotickStartSettings* pSettings) const;
	EGenotickResult ReleaseInternal() const;

	CJavaLoaderGenotick& m_javaLoader;
	JavaVM& m_javaVM;
	JNIEnv& m_javaEnv;

	struct MainInterfaceTag { static constexpr auto Name() { return "com/alphatica/genotick/genotick/MainInterface"; } };
	using TMainInterfaceClass = jni::Class<MainInterfaceTag>;
	using TJavaStringClass = jni::Class<jni::StringTag>;
	using TJavaArrayOfStrings = jni::Array<jni::String>;

	using TGetInterfaceVersionMethodType = jni::jint();
	using TGetInterfaceVersionMethod = jni::StaticMethod<MainInterfaceTag, TGetInterfaceVersionMethodType>;

	using TStartMethodType = jni::jint(TJavaArrayOfStrings);
	using TStartMethod = jni::StaticMethod<MainInterfaceTag, TStartMethodType>;

	jni::UniqueClass<TMainInterfaceClass::TagType> m_mainInterfaceClass;
	jni::UniqueClass<TJavaStringClass::TagType> m_stringClass;
	TGetInterfaceVersionMethod m_getInterfaceVersionMethod;
	TStartMethod m_startMethod;
};
