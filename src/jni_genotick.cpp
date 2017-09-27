
#include "jni_genotick.h"
#include "jni_error.h"
#include "genotick_error.h"
#include "java_loader.h"
#include "utils.h"
#include <string>

CJniGenotick::CJniGenotick(CJavaLoader* pJavaLoader, JavaVM* pJavaVM, JNIEnv* pJavaEnv)
	: m_javaLoader(*static_cast<CJavaLoaderGenotick*>(pJavaLoader))
	, m_javaVM(*pJavaVM)
	, m_javaEnv(*pJavaEnv)
	, m_mainInterfaceClass(TMainInterfaceClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
	, m_stringClass(TJavaStringClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
	, m_getInterfaceVersionMethod(m_mainInterfaceClass->GetStaticMethod<TGetInterfaceVersionMethodType>(m_javaEnv, "getInterfaceVersion"))
	, m_startMethod(m_mainInterfaceClass->GetStaticMethod<TStartMethodType>(m_javaEnv, "start"))
{
	functions = new IGenotickFunctions_;
	struct IGenotickFunctions_* mutableFunctions = const_cast<IGenotickFunctions_*>(functions);
	mutableFunctions->GetInterfaceVersion = GetInterfaceVersionThis;
	mutableFunctions->Start = StartThis;
	mutableFunctions->Release = ReleaseThis;
}

CJniGenotick::~CJniGenotick()
{
	SAFE_DELETE(functions);
}

jint_t CJniGenotick::GetInterfaceVersionInternal() const
{
	return m_mainInterfaceClass->Call(m_javaEnv, m_getInterfaceVersionMethod);
}

EGenotickResult CJniGenotick::StartInternal(const SGenotickStartSettings* pSettings) const
{
	if (!pSettings)
		return eGenotickResult_InvalidArgument;

	try
	{
		const jni::jsize length = pSettings->parameterCount;
		TJavaArrayOfStrings args = TJavaArrayOfStrings::New(m_javaEnv, length, *m_stringClass.get());
		for (jni::jsize i = 0; i < length; ++i)
		{
			const char* parameter = pSettings->parameters[i];
			jni::String newString = jni::Make<jni::String>(m_javaEnv, parameter);
			args.Set(m_javaEnv, i, newString);
		}
		const jni::jint error = m_mainInterfaceClass->Call(m_javaEnv, m_startMethod, args);
		return genotick::ErrorCodeToGenotickResult(error);
	}
	catch (jni::PendingJavaException)
	{
		jni::Throwable exception(jni::ExceptionOccurred(m_javaEnv)); // TODO get and store exception description
		jni::ExceptionDescribe(m_javaEnv);
		jni::ExceptionClear(m_javaEnv);
		return eGenotickResult_JavaException;
	}
}

EGenotickResult CJniGenotick::ReleaseInternal() const
{
	const jint jniResult = m_javaVM.DestroyJavaVM();
	assert(jniResult == JNI_OK);
	if (jniResult == JNI_OK)
	{
		m_javaLoader.RemoveInstance(this);
	}
	return jni::JniErrorToGenotickResult(jniResult);
}
