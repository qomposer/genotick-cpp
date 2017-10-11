
#include "genotick.h"
#include <genotick/jni/error.h>
#include <genotick/loader.h>
#include <utils.h>
#include <string>

#define MISMATCH_MESSAGE "Mismatching Data Type Found"
static_assert(sizeof(TGenotickInt32) == sizeof(::jni::jint), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickInt64) == sizeof(::jni::jlong), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickByte) == sizeof(::jni::jbyte), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickBoolean) == sizeof(::jni::jboolean), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickChar) == sizeof(::jni::jchar), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickShort) == sizeof(::jni::jshort), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickFloat) == sizeof(::jni::jfloat), MISMATCH_MESSAGE);
static_assert(sizeof(TGenotickDouble) == sizeof(::jni::jdouble), MISMATCH_MESSAGE);
static_assert(sizeof(EGenotickWeightMode) == sizeof(TGenotickInt32), MISMATCH_MESSAGE);
static_assert(sizeof(EGenotickInheritedWeightMode) == sizeof(TGenotickInt32), MISMATCH_MESSAGE);
static_assert(sizeof(EGenotickChartMode) == sizeof(TGenotickInt32), MISMATCH_MESSAGE);
static_assert(sizeof(EGenotickPrediction) == sizeof(TGenotickInt32), MISMATCH_MESSAGE);
#undef MISMATCH_MESSAGE

namespace genotick {
namespace jni {

CGenotick::CGenotick(CLoader* pLoader, ::jni::JavaVM* pJavaVM, ::jni::JNIEnv* pJavaEnv)
	: m_loader(*static_cast<CLoaderFriend*>(pLoader))
	, m_javaVM(*pJavaVM)
	, m_javaEnv(*pJavaEnv)
	, m_stringClass(::jni::StringClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
	, m_mainInterface(pJavaEnv)
	, m_mainSettings(pJavaEnv)
	, m_dataLines(pJavaEnv)
	, m_mainAppData(pJavaEnv)
	, m_timePoint(pJavaEnv)
	, m_timePoints(pJavaEnv)
	, m_weightMode(pJavaEnv)
	, m_inheritedWeightMode(pJavaEnv)
	, m_chartMode(pJavaEnv)
	, m_errorCode(pJavaEnv)
	, m_prediction(pJavaEnv)
	, m_predictions(pJavaEnv)
{
	SGenotickFunctions& mutableFunctions = const_cast<SGenotickFunctions&>(functions);
	mutableFunctions.GetInterfaceVersion = GetInterfaceVersion;
	mutableFunctions.CreateSession = CreateSession;
	mutableFunctions.RemoveSession = RemoveSession;
	mutableFunctions.RemoveAllSessions = RemoveAllSessions;
	mutableFunctions.GetSettings = GetSettings;
	mutableFunctions.ChangeSettings = ChangeSettings;
	mutableFunctions.SetAssetData = SetAssetData;
	mutableFunctions.Start = Start;
	mutableFunctions.GetTimePoints = GetTimePoints;
	mutableFunctions.GetPredictions = GetPredictions;
	mutableFunctions.GetNewestTimePoint = GetNewestTimePoint;
	mutableFunctions.GetNewestPrediction = GetNewestPrediction;
	mutableFunctions.Release = Release;

	utils::VerifyFunctionsStruct(functions);
}

CGenotick::~CGenotick()
{
}

TGenotickInt32 CGenotick::GetInterfaceVersionInternal() const
{
	return m_mainInterface.getInterfaceVersion();
}

EGenotickResult CGenotick::CreateSessionInternal(TGenotickSessionId sessionId) const
{
	return EGenotickResult::ErrorInvalidSession;
}

EGenotickResult CGenotick::RemoveSessionInternal(TGenotickSessionId sessionId) const
{
	return EGenotickResult::ErrorInvalidSession;
}

EGenotickResult CGenotick::RemoveAllSessionsInternal() const
{
	return EGenotickResult::ErrorInvalidSession;
}

#define GENOTICK_UNROLL_FIELDS_TO_NATIVE(TYPE, NAME) { \
ToNative(pSettings->NAME, this->m_mainSettings.Get_##NAME(settingsObject)); }

#define GENOTICK_UNROLL_FIELDS_TO_JAVA(TYPE, NAME) { \
auto value = ToJava<typename remote::CMainSettings::TYPE::FieldType>(pSettings->NAME); \
this->m_mainSettings.Set_##NAME(settingsObject, value); }

EGenotickResult CGenotick::GetSettingsInternal(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const
{
	if (!pSettings)
		return EGenotickResult::InvalidArgument;

	try
	{
		const remote::CMainSettings::TObject settingsObject = m_mainInterface.getSettings(sessionId);
		if (settingsObject.Get() == nullptr)
			return EGenotickResult::ErrorInvalidSession;

		GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_FIELDS_TO_NATIVE);
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(m_javaEnv, exception);
	}
	catch (const EnumMismatchException& exception)
	{
		return HandleEnumMismatchException(exception);
	}
}

EGenotickResult CGenotick::ChangeSettingsInternal(TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) const
{
	if (!pSettings)
		return EGenotickResult::InvalidArgument;

	try
	{
		const remote::CMainSettings::TObject settingsObject = m_mainInterface.getSettings(sessionId);
		if (settingsObject.Get() == nullptr)
			return EGenotickResult::ErrorInvalidSession;

		GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_FIELDS_TO_JAVA);
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(m_javaEnv, exception);
	}
	catch (const EnumMismatchException& exception)
	{
		return HandleEnumMismatchException(exception);
	}
}

#undef GENOTICK_UNROLL_FIELDS_TO_NATIVE
#undef GENOTICK_UNROLL_FIELDS_TO_JAVA


EGenotickResult CGenotick::SetAssetDataInternal(TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) const
{
	return EGenotickResult::ErrorInvalidSession;
}

EGenotickResult CGenotick::StartInternal(TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) const
{
	if (!pArgs)
		return EGenotickResult::InvalidArgument;

	try
	{
		const ::jni::jsize count = pArgs->elementCount;
		::jni::StringArray args = ::jni::StringArray::New(m_javaEnv, count, *m_stringClass.get());
		for (::jni::jsize i = 0; i < count; ++i)
		{
			const char* parameter = pArgs->elements[i];
			::jni::String newString = ::jni::Make<::jni::String>(m_javaEnv, parameter);
			args.Set(m_javaEnv, i, newString);
		}
		const ::jni::jint error = m_mainInterface.start(sessionId, args);
		return remote::ErrorCodeToGenotickResult(error);
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(m_javaEnv, exception);
	}
}


EGenotickResult CGenotick::GetTimePointsInternal(TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) const
{
	return EGenotickResult::ErrorInvalidSession;
}

EGenotickResult CGenotick::GetPredictionsInternal(TGenotickSessionId sessionId, IGenotickPredictions** ppPredictions) const
{
	return EGenotickResult::ErrorInvalidSession;
}

EGenotickResult CGenotick::GetNewestTimePointInternal(TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) const
{
	return EGenotickResult::ErrorInvalidSession;
}

EGenotickResult CGenotick::GetNewestPredictionInternal(TGenotickSessionId sessionId, EGenotickPrediction* pPrediction) const
{
	return EGenotickResult::ErrorInvalidSession;
}


EGenotickResult CGenotick::ReleaseInternal() const
{
	return m_loader.RemoveInstance(this, m_javaVM);
}

} // namespace jni
} // namespace genotick
