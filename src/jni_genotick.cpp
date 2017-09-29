
#include "jni_genotick.h"
#include "jni_error.h"
#include "jni_genotick_error.h"
#include "jni_genotick_settings.h"
#include "java_loader.h"
#include "utils.h"
#include <string>

#define MISMATCH_MESSAGE "Mismatching Data Type Found"
static_assert(sizeof(TGenotickInt32) == sizeof(jni::jint), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickInt64) == sizeof(jni::jlong), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickByte) == sizeof(jni::jbyte), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickBoolean) == sizeof(jni::jboolean), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickChar) == sizeof(jni::jchar), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickShort) == sizeof(jni::jshort), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickFloat) == sizeof(jni::jfloat), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickDouble) == sizeof(jni::jdouble), MISMATCH_MESSAGE);
static_assert(sizeof(EGenotickWeightMode) == sizeof(jni::jint), MISMATCH_MESSAGE);
static_assert(sizeof(EGenotickInheritedWeightMode) == sizeof(jni::jint), MISMATCH_MESSAGE);
static_assert(sizeof(EGenotickChartMode) == sizeof(jni::jint), MISMATCH_MESSAGE);
#undef MISMATCH_MESSAGE

namespace jni {
namespace genotick {

CGenotick::CGenotick(CJavaLoader* pJavaLoader, jni::JavaVM* pJavaVM, jni::JNIEnv* pJavaEnv)
	: m_javaLoader(*static_cast<CJavaLoaderGenotick*>(pJavaLoader))
	, m_javaVM(*pJavaVM)
	, m_javaEnv(*pJavaEnv)
	, m_stringClass(jni::StringClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
	, m_mainInterface(pJavaEnv)
	, m_mainSettings(pJavaEnv)
	, m_timePoint(pJavaEnv)
	, m_weightMode(pJavaEnv)
	, m_inheritedWeightMode(pJavaEnv)
	, m_chartMode(pJavaEnv)
{
	functions = new IGenotickFunctions;
	IGenotickFunctions* mutableFunctions = const_cast<IGenotickFunctions*>(functions);
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
	return m_mainInterface.getInterfaceVersion();
}

#define GENOTICK_UNROLL_FIELDS_TO_NATIVE(TYPE, NAME) { \
ToNative(pSettings->NAME, this->m_mainSettings.Get_##NAME(settingsObject)); }

#define GENOTICK_UNROLL_FIELDS_TO_JAVA(TYPE, NAME) { \
auto value = ToJava<typename jni::genotick::CMainSettings::TYPE::FieldType>(pSettings->NAME); \
this->m_mainSettings.Set_##NAME(settingsObject, value); }

EGenotickResult CGenotick::GetSettingsInternal(SGenotickMainSettings* pSettings)
{
	if (!pSettings)
		return eGenotickResult_InvalidArgument;

	try
	{
		const jni::genotick::CMainSettings::TObject settingsObject = m_mainInterface.getSettings();
		GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_FIELDS_TO_NATIVE);
		return eGenotickResult_Success;
	}
	catch (jni::PendingJavaException)
	{
		return HandleJavaException();
	}
	catch (EnumMismatchException)
	{
		return HandleEnumMismatchException();
	}
}

EGenotickResult CGenotick::ChangeSettingsInternal(const SGenotickMainSettings* pSettings)
{
	if (!pSettings)
		return eGenotickResult_InvalidArgument;

	try
	{
		const jni::genotick::CMainSettings::TObject settingsObject = m_mainInterface.getSettings();
		GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_FIELDS_TO_JAVA);
		return eGenotickResult_Success;
	}
	catch (jni::PendingJavaException)
	{
		return HandleJavaException();
	}
	catch (EnumMismatchException)
	{
		return HandleEnumMismatchException();
	}
}

#undef GENOTICK_UNROLL_FIELDS_TO_NATIVE
#undef GENOTICK_UNROLL_FIELDS_TO_JAVA

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
		const jni::jint error = m_mainInterface.start(args);
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
	jni::ThrowableObject exception(jni::ExceptionOccurred(m_javaEnv)); // TODO get and store exception description
	jni::ExceptionDescribe(m_javaEnv);
	jni::ExceptionClear(m_javaEnv);
	return eGenotickResult_JavaException;
}

EGenotickResult CGenotick::HandleEnumMismatchException()
{
	return eGenotickResult_JavaEnumMismatch;
}

} // namespace genotick
} // namespace jni
