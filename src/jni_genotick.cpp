
#include "jni_genotick.h"
#include "jni_error.h"
#include "jni_genotick_error.h"
#include "jni_genotick_settings.h"
#include "java_loader.h"
#include "utils.h"
#include <string>

namespace jni {
namespace genotick {

CGenotick::CGenotick(CJavaLoader* pJavaLoader, jni::JavaVM* pJavaVM, jni::JNIEnv* pJavaEnv)
	: m_javaLoader(*static_cast<CJavaLoaderGenotick*>(pJavaLoader))
	, m_javaVM(*pJavaVM)
	, m_javaEnv(*pJavaEnv)
	, m_stringClass(jni::StringClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
	, m_mainInterface(pJavaEnv)
	, m_mainSettings(pJavaEnv)
{
	functions = new IGenotickFunctions_;
	IGenotickFunctions_* mutableFunctions = const_cast<IGenotickFunctions_*>(functions);
	mutableFunctions->GetInterfaceVersion = GetInterfaceVersionThis;
	mutableFunctions->GetSettings = GetSettingsThis;
	mutableFunctions->ChangeSettings = ChangeSettingsThis;
	mutableFunctions->Start = StartThis;
	mutableFunctions->Release = ReleaseThis;
}

CGenotick::~CGenotick()
{
	SAFE_DELETE(functions);
}

TGenotickInt32 CGenotick::GetInterfaceVersionInternal()
{
	return m_mainInterface.GetInterfaceVersion();
}

EGenotickResult CGenotick::GetSettingsInternal(SGenotickMainSettings* pSettings)
{
	if (!pSettings)
		return eGenotickResult_InvalidArgument;




	return eGenotickResult_Success;
}

EGenotickResult CGenotick::ChangeSettingsInternal(const SGenotickMainSettings* pSettings)
{
	if (!pSettings)
		return eGenotickResult_InvalidArgument;




	return eGenotickResult_Success;
}

EGenotickResult CGenotick::StartInternal(const SGenotickStartSettings* pSettings)
{
	if (!pSettings)
		return eGenotickResult_InvalidArgument;

	try
	{
		const jni::jsize length = pSettings->parameterCount;
		jni::StringArray args = jni::StringArray::New(m_javaEnv, length, *m_stringClass.get());
		for (jni::jsize i = 0; i < length; ++i)
		{
			const char* parameter = pSettings->parameters[i];
			jni::String newString = jni::Make<jni::String>(m_javaEnv, parameter);
			args.Set(m_javaEnv, i, newString);
		}
		const jni::jint error = m_mainInterface.Start(args);
		return jni::genotick::ErrorCodeToGenotickResult(error);
	}
	catch (jni::PendingJavaException)
	{
		return HandleJavaException();
	}
}

EGenotickResult CGenotick::ReleaseInternal()
{
	const jint jniResult = m_javaVM.DestroyJavaVM();
	assert(jniResult == JNI_OK);
	if (jniResult == JNI_OK)
	{
		m_javaLoader.RemoveInstance(this);
	}
	return jni::JniErrorToGenotickResult(jniResult);
}

EGenotickResult CGenotick::HandleJavaException()
{
	jni::Throwable exception(jni::ExceptionOccurred(m_javaEnv)); // TODO get and store exception description
	jni::ExceptionDescribe(m_javaEnv);
	jni::ExceptionClear(m_javaEnv);
	return eGenotickResult_JavaException;
}

} // namespace genotick
} // namespace jni
