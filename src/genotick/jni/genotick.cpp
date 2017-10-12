
#include "genotick.h"
#include <genotick/jni/error.h>
#include <genotick/jni/genotick_predictions.h>
#include <genotick/jni/genotick_timepoints.h>
#include <utils.h>
#include <cassert>

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
	, m_remoteString(::jni::StringClass::Find(m_javaEnv).NewGlobalRef(m_javaEnv))
	, m_remoteMainInterface(pJavaEnv)
	, m_remoteMainSettings(pJavaEnv)
	, m_remoteDataLines(pJavaEnv)
	, m_remoteMainAppData(pJavaEnv)
	, m_remoteTimePoint(pJavaEnv)
	, m_remoteTimePoints(pJavaEnv)
	, m_remoteWeightMode(pJavaEnv)
	, m_remoteInheritedWeightMode(pJavaEnv)
	, m_remoteChartMode(pJavaEnv)
	, m_remoteErrorCode(pJavaEnv)
	, m_remotePrediction(pJavaEnv)
	, m_remotePredictions(pJavaEnv)
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

	::utils::VerifyFunctionsStruct(functions);
}

CGenotick::~CGenotick()
{
}

TGenotickInt32 CGenotick::GetInterfaceVersionInternal() const
{
	return m_remoteMainInterface.getInterfaceVersion();
}

EGenotickResult CGenotick::CreateSessionInternal(TGenotickSessionId sessionId) const
{
	const remote::CErrorCode::TObject errorObject = m_remoteMainInterface.createSession(sessionId);
	return remote::ErrorCodeToGenotickResult(m_remoteErrorCode, errorObject);
}

EGenotickResult CGenotick::RemoveSessionInternal(TGenotickSessionId sessionId) const
{
	const remote::CErrorCode::TObject errorObject = m_remoteMainInterface.clearSession(sessionId);
	return remote::ErrorCodeToGenotickResult(m_remoteErrorCode, errorObject);
}

EGenotickResult CGenotick::RemoveAllSessionsInternal() const
{
	m_remoteMainInterface.clearSessions();
	return EGenotickResult::Success;
}

#define GENOTICK_UNROLL_FIELDS_TO_NATIVE(TYPE, NAME) { \
ToNative(pSettings->NAME, this->m_remoteMainSettings.Get_##NAME(settingsObject)); }

#define GENOTICK_UNROLL_FIELDS_TO_JAVA(TYPE, NAME) { \
auto value = ToJava<typename remote::CMainSettings::TYPE::FieldType>(pSettings->NAME); \
this->m_remoteMainSettings.Set_##NAME(settingsObject, value); }

EGenotickResult CGenotick::GetSettingsInternal(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const
{
	if (!pSettings)
		return EGenotickResult::InvalidArgument;

	try
	{
		const remote::CMainSettings::TObject settingsObject = m_remoteMainInterface.getSettings(sessionId);
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
		const remote::CMainSettings::TObject settingsObject = m_remoteMainInterface.getSettings(sessionId);
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
		::jni::StringArray args = ::jni::StringArray::New(m_javaEnv, count, *m_remoteString.get());
		for (::jni::jsize i = 0; i < count; ++i)
		{
			const char* parameter = pArgs->elements[i];
			::jni::String jniParameter = ::jni::Make<::jni::String>(m_javaEnv, parameter);
			args.Set(m_javaEnv, i, jniParameter);
		}
		const remote::CErrorCode::TObject errorObject = m_remoteMainInterface.start(sessionId, args);
		return remote::ErrorCodeToGenotickResult(m_remoteErrorCode, errorObject);
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(m_javaEnv, exception);
	}
}


EGenotickResult CGenotick::GetTimePointsInternal(TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) const
{
	assert(ppTimePoints != nullptr);

	const remote::CTimePoints::TObject object = m_remoteMainInterface.getTimePoints(sessionId);
	if (object.Get() == nullptr)
		return EGenotickResult::ErrorInvalidSession;

	*ppTimePoints = new CGenotickTimePoints(object, m_remoteTimePoints, m_remoteTimePoint);
	return EGenotickResult::Success;
}

EGenotickResult CGenotick::GetPredictionsInternal(TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) const
{
	assert(ppPredictions != nullptr);

	const ::jni::String jniAssetName = ::jni::Make<::jni::String>(m_javaEnv, assetName);
	const remote::CPredictions::TObject object = m_remoteMainInterface.getPredictions(sessionId, jniAssetName);
	if (object.Get() == nullptr)
		return EGenotickResult::ErrorInvalidSession;

	*ppPredictions = new CGenotickPredictions(object, m_remotePredictions, m_remotePrediction);
	return EGenotickResult::Success;
}

EGenotickResult CGenotick::GetNewestTimePointInternal(TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) const
{
	assert(pTimePoint != nullptr);

	const remote::CTimePoint::TObject object = m_remoteMainInterface.getNewestTimePoint(sessionId);
	if (object.Get() == nullptr)
		return EGenotickResult::ErrorInvalidSession;

	*pTimePoint = m_remoteTimePoint.getValue(object);
	return EGenotickResult::Success;
}

EGenotickResult CGenotick::GetNewestPredictionInternal(TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) const
{
	assert(pPrediction != nullptr);

	const ::jni::String jniAssetName = ::jni::Make<::jni::String>(m_javaEnv, assetName);
	const remote::CPrediction::TObject object = m_remoteMainInterface.getNewestPrediction(sessionId, jniAssetName);
	if (object.Get() == nullptr)
		return EGenotickResult::ErrorInvalidSession;

	*pPrediction = EGenotickPrediction::get_by_value(m_remotePrediction.getValue(object));
	return EGenotickResult::Success;
}


EGenotickResult CGenotick::ReleaseInternal() const
{
	return m_loader.RemoveInstance(this, m_javaVM);
}

} // namespace jni
} // namespace genotick
