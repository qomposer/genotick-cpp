
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

CGenotick::CGenotick(CLoader& loader, ::jni::JavaVM& javaVM, ::jni::JNIEnv& javaEnv)
	: m_loader(static_cast<CLoaderFriend&>(loader))
	, m_javaVM(javaVM)
	, m_javaEnv(javaEnv)
	, m_remoteString(::jni::StringClass::Find(javaEnv).NewGlobalRef(javaEnv))
	, m_remoteMainInterface(javaEnv)
	, m_remoteMainSettings(javaEnv)
	, m_remoteDataLines(javaEnv)
	, m_remoteMainAppData(javaEnv)
	, m_remoteTimePoint(javaEnv)
	, m_remoteTimePoints(javaEnv)
	, m_remoteWeightMode(javaEnv)
	, m_remoteInheritedWeightMode(javaEnv)
	, m_remoteChartMode(javaEnv)
	, m_remoteErrorCode(javaEnv)
	, m_remotePrediction(javaEnv)
	, m_remotePredictions(javaEnv)
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
ToNative(pSettings->NAME, this->m_remoteMainSettings.Get_##NAME(jniSettings)); }

#define GENOTICK_UNROLL_FIELDS_TO_JAVA(TYPE, NAME) { \
auto value = ToJava<typename remote::CMainSettings::TYPE::FieldType>(pSettings->NAME); \
this->m_remoteMainSettings.Set_##NAME(jniSettings, value); }

EGenotickResult CGenotick::GetSettingsInternal(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const
{
	if (pSettings == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const remote::CMainSettings::TObject jniSettings = m_remoteMainInterface.getSettings(sessionId);
		if (jniSettings.Get() == nullptr)
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
	if (pSettings == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const remote::CMainSettings::TObject jniSettings = m_remoteMainInterface.getSettings(sessionId);
		if (jniSettings.Get() == nullptr)
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
	if (pAssetData == nullptr || pAssetData->dataPoints == nullptr || pAssetData->assetName == nullptr || *pAssetData->assetName == 0)
		return EGenotickResult::InvalidArgument;

	if (pAssetData->dataPointCount == 0)
		return EGenotickResult::ErrorInsufficientData;

	const remote::CMainAppData::TObject mainAppDataObject = m_remoteMainInterface.getData(sessionId);
	if (mainAppDataObject.Get() == nullptr)
		return EGenotickResult::ErrorInvalidSession;

	const ::jni::jint lineCount = static_cast<::jni::jint>(pAssetData->dataPointCount);
	const ::jni::jint otherColumnCount = static_cast<::jni::jint>(pAssetData->dataPoints[0].otherColumnCount);
	const ::jni::jint columnCount = static_cast<::jni::jint>(GenotickMinColumnCount) + otherColumnCount;
	const ::jni::jboolean firstIsNewest = pAssetData->firstDataPointIsNewest;
	const ::jni::String jniAssetName = ::jni::Make<::jni::String>(m_javaEnv, pAssetData->assetName);

	remote::CDataLines::TObject jniDataLines = m_remoteDataLines.New(lineCount, columnCount, firstIsNewest);
	if (jniDataLines.Get() == nullptr)
		return EGenotickResult::ErrorInsufficientData;

	for (::jni::jint line = 0; line < lineCount; ++line)
	{
		const TGenotickDataPoint& dataPoint = pAssetData->dataPoints[line];

		assert(static_cast<::jni::jint>(dataPoint.otherColumnCount) == otherColumnCount);

		m_remoteDataLines.setTime(jniDataLines, line, dataPoint.time);
		m_remoteDataLines.setOpen(jniDataLines, line, dataPoint.open);
		m_remoteDataLines.setHigh(jniDataLines, line, dataPoint.high);
		m_remoteDataLines.setLow(jniDataLines, line, dataPoint.low);
		m_remoteDataLines.setClose(jniDataLines, line, dataPoint.close);
		m_remoteDataLines.setVolume(jniDataLines, line, dataPoint.volume);

		for (::jni::jint column = 0; column < otherColumnCount; ++column)
		{
			m_remoteDataLines.setOther(jniDataLines, line, column, dataPoint.otherColumns[column]);
		}
	}

	m_remoteMainAppData.put(mainAppDataObject, jniAssetName, jniDataLines);

	return EGenotickResult::Success;
}

EGenotickResult CGenotick::StartInternal(TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) const
{
	if (pArgs == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const ::jni::jsize count = pArgs->elementCount;
		::jni::StringArray args = ::jni::StringArray::New(m_javaEnv, count, *m_remoteString.get());
		for (::jni::jsize index = 0; index < count; ++index)
		{
			const char* parameter = pArgs->elements[index];
			::jni::String jniParameter = ::jni::Make<::jni::String>(m_javaEnv, parameter);
			args.Set(m_javaEnv, index, jniParameter);
		}
		const remote::CErrorCode::TObject jniErrorCode = m_remoteMainInterface.start(sessionId, args);
		return remote::ErrorCodeToGenotickResult(m_remoteErrorCode, jniErrorCode);
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(m_javaEnv, exception);
	}
}

EGenotickResult CGenotick::GetTimePointsInternal(TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) const
{
	if (ppTimePoints == nullptr)
		return EGenotickResult::InvalidArgument;

	const remote::CTimePoints::TObject jniTimePoints = m_remoteMainInterface.getTimePoints(sessionId);
	if (jniTimePoints.Get() == nullptr)
		return EGenotickResult::ErrorInvalidSession;

	*ppTimePoints = new CGenotickTimePoints(jniTimePoints, m_remoteTimePoints, m_remoteTimePoint);
	return EGenotickResult::Success;
}

EGenotickResult CGenotick::GetPredictionsInternal(TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) const
{
	if (ppPredictions == nullptr)
		return EGenotickResult::InvalidArgument;

	const ::jni::String jniAssetName = ::jni::Make<::jni::String>(m_javaEnv, assetName);
	const remote::CPredictions::TObject jniPredictions = m_remoteMainInterface.getPredictions(sessionId, jniAssetName);
	if (jniPredictions.Get() == nullptr)
		return EGenotickResult::ErrorInvalidSession;

	*ppPredictions = new CGenotickPredictions(jniPredictions, m_remotePredictions, m_remotePrediction);
	return EGenotickResult::Success;
}

EGenotickResult CGenotick::GetNewestTimePointInternal(TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) const
{
	if (pTimePoint == nullptr)
		return EGenotickResult::InvalidArgument;

	const remote::CTimePoint::TObject jniTimePoint = m_remoteMainInterface.getNewestTimePoint(sessionId);
	if (jniTimePoint.Get() == nullptr)
		return EGenotickResult::ErrorInvalidSession;

	*pTimePoint = m_remoteTimePoint.getValue(jniTimePoint);
	return EGenotickResult::Success;
}

EGenotickResult CGenotick::GetNewestPredictionInternal(TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) const
{
	if (pPrediction == nullptr)
		return EGenotickResult::InvalidArgument;

	const ::jni::String jniAssetName = ::jni::Make<::jni::String>(m_javaEnv, assetName);
	const remote::CPrediction::TObject jniPrediction = m_remoteMainInterface.getNewestPrediction(sessionId, jniAssetName);
	if (jniPrediction.Get() == nullptr)
		return EGenotickResult::ErrorInvalidSession;

	const ::jni::jint predictionValue = m_remotePrediction.getValue(jniPrediction);
	*pPrediction = EGenotickPrediction::get_by_value(predictionValue);
	return EGenotickResult::Success;
}


EGenotickResult CGenotick::ReleaseInternal() const
{
	return m_loader.RemoveInstance(this, m_javaVM);
}

} // namespace jni
} // namespace genotick
