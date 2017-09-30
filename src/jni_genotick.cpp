
#include "jni_genotick.h"
#include "jni_error.h"
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
static_assert(sizeof(EGenotickResult) == sizeof(jni::jint), MISMATCH_MESSAGE);
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
	, m_errorCode(pJavaEnv)
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
		return EGenotickResult::InvalidArgument;

	try
	{
		const jni::genotick::CMainSettings::TObject settingsObject = m_mainInterface.getSettings();
		GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_FIELDS_TO_NATIVE);
		return EGenotickResult::Success;
	}
	catch (const jni::PendingJavaException& exception)
	{
		return jni::HandleJavaException(m_javaEnv, exception);
	}
	catch (const jni::EnumMismatchException& exception)
	{
		return jni::HandleEnumMismatchException(exception);
	}
}

EGenotickResult CGenotick::ChangeSettingsInternal(const SGenotickMainSettings* pSettings)
{
	if (!pSettings)
		return EGenotickResult::InvalidArgument;

	try
	{
		const jni::genotick::CMainSettings::TObject settingsObject = m_mainInterface.getSettings();
		GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_FIELDS_TO_JAVA);
		return EGenotickResult::Success;
	}
	catch (const jni::PendingJavaException& exception)
	{
		return jni::HandleJavaException(m_javaEnv, exception);
	}
	catch (const jni::EnumMismatchException& exception)
	{
		return jni::HandleEnumMismatchException(exception);
	}
}

#undef GENOTICK_UNROLL_FIELDS_TO_NATIVE
#undef GENOTICK_UNROLL_FIELDS_TO_JAVA

EGenotickResult CGenotick::StartInternal(const SGenotickStartSettings* pSettings)
{
	if (!pSettings)
		return EGenotickResult::InvalidArgument;

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
	catch (const jni::PendingJavaException& exception)
	{
		return jni::HandleJavaException(m_javaEnv, exception);
	}
}

EGenotickResult CGenotick::ReleaseInternal()
{
	return m_javaLoader.RemoveInstance(this, m_javaVM);
}

} // namespace genotick
} // namespace jni
