
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

CGenotick::CGenotick(CLoader& loader, JavaVM& javaVM, JNIEnv& javaEnv)
	: m_loader(static_cast<CLoaderFriend&>(loader))
	, m_javaVM(javaVM)
{
	SGenotickFunctions& mutableFunctions = const_cast<SGenotickFunctions&>(functions);
	memset(&mutableFunctions, 0, sizeof(mutableFunctions));
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
	mutableFunctions.AttachCurrentThread = AttachCurrentThread;
	mutableFunctions.DetachCurrentThread = DetachCurrentThread;
	mutableFunctions.Release = Release;

	::utils::VerifyFunctionsStruct(functions);
	::utils::VerifyEqualPointers(&functions, static_cast<IGenotick*>(this));

	AddThreadData(javaEnv);
}

CGenotick::~CGenotick()
{
}

TGenotickInt32 CGenotick::GetInterfaceVersionInternal() const
{
	try
	{
		return GetThreadData().remoteMainInterface.getInterfaceVersion();
	}
	catch (const UnrecognizedThreadException& exception)
	{
		HandleUnrecognizedThreadException(exception);
		return 0;
	}
}

EGenotickResult CGenotick::CreateSessionInternal(TGenotickSessionId sessionId) const
{
	try
	{
		const SThreadData& threadData = GetThreadData();
		const remote::CErrorCode::TObject errorObject = threadData.remoteMainInterface.createSession(sessionId);
		return remote::ErrorCodeToGenotickResult(threadData.remoteErrorCode, errorObject);
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

EGenotickResult CGenotick::RemoveSessionInternal(TGenotickSessionId sessionId) const
{
	try
	{
		const SThreadData& threadData = GetThreadData();
		const remote::CErrorCode::TObject errorObject = threadData.remoteMainInterface.clearSession(sessionId);
		return remote::ErrorCodeToGenotickResult(threadData.remoteErrorCode, errorObject);
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

EGenotickResult CGenotick::RemoveAllSessionsInternal() const
{
	try
	{
		GetThreadData().remoteMainInterface.clearSessions();
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}


#define GENOTICK_UNROLL_FIELDS_TO_NATIVE(type, name) { \
this->ToNative(pSettings->name, threadData.remoteMainSettings.Get_##name(jniSettings)); }

#define GENOTICK_UNROLL_FIELDS_TO_JAVA(type, name) { \
auto value = ToJava<typename remote::CMainSettings::type::FieldType>(pSettings->name); \
threadData.remoteMainSettings.Set_##name(jniSettings, value); }

EGenotickResult CGenotick::GetSettingsInternal(TGenotickSessionId sessionId, TGenotickMainSettings* pSettings) const
{
	if (pSettings == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const SThreadData& threadData = GetThreadData();
		const remote::CMainSettings::TObject jniSettings = threadData.remoteMainInterface.getSettings(sessionId);
		if (jniSettings.Get() == nullptr)
			return EGenotickResult::ErrorInvalidSession;

		GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_FIELDS_TO_NATIVE);
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const EnumMismatchException& exception)
	{
		return HandleEnumMismatchException(exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

EGenotickResult CGenotick::ChangeSettingsInternal(TGenotickSessionId sessionId, const TGenotickMainSettings* pSettings) const
{
	if (pSettings == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const SThreadData& threadData = GetThreadData();
		const remote::CMainSettings::TObject jniSettings = threadData.remoteMainInterface.getSettings(sessionId);
		if (jniSettings.Get() == nullptr)
			return EGenotickResult::ErrorInvalidSession;

		GENOTICK_MAINSETTINGS_FIELDS(GENOTICK_UNROLL_FIELDS_TO_JAVA);
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const EnumMismatchException& exception)
	{
		return HandleEnumMismatchException(exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

#undef GENOTICK_UNROLL_FIELDS_TO_NATIVE
#undef GENOTICK_UNROLL_FIELDS_TO_JAVA


EGenotickResult CGenotick::SetAssetDataInternal(TGenotickSessionId sessionId, const TGenotickAssetData* pAssetData) const
{
	if (pAssetData == nullptr ||
		pAssetData->dataPoints == nullptr ||
		pAssetData->assetName == nullptr ||
		*pAssetData->assetName == 0)
		return EGenotickResult::InvalidArgument;

	if (pAssetData->dataPointCount == 0)
		return EGenotickResult::ErrorInsufficientData;

	try
	{
		const SThreadData& threadData = GetThreadData();
		const remote::CMainAppData::TObject mainAppDataObject = threadData.remoteMainInterface.getData(sessionId);
		if (mainAppDataObject.Get() == nullptr)
			return EGenotickResult::ErrorInvalidSession;

		const ::jni::jint lineCount = static_cast<::jni::jint>(pAssetData->dataPointCount);
		const ::jni::jint optionalColumnCount = static_cast<::jni::jint>(pAssetData->optionalColumnCount);
		const ::jni::jint columnCount = static_cast<::jni::jint>(GenotickMinColumnCount) + optionalColumnCount;
		const ::jni::jboolean firstIsNewest = pAssetData->firstDataPointIsNewest;
		const ::jni::String jniAssetName = ::jni::Make<::jni::String>(threadData.javaEnv, pAssetData->assetName);

		remote::CDataLines::TObject jniDataLines = threadData.remoteDataLines.New(lineCount, columnCount, firstIsNewest);
		if (jniDataLines.Get() == nullptr)
			return EGenotickResult::ErrorInsufficientData;

		for (::jni::jint line = 0; line < lineCount; ++line)
		{
			const TGenotickDataPoint& dataPoint = pAssetData->dataPoints[line];

			threadData.remoteDataLines.setTime(jniDataLines, line, dataPoint.time);
			threadData.remoteDataLines.setOpen(jniDataLines, line, dataPoint.open);
			threadData.remoteDataLines.setHigh(jniDataLines, line, dataPoint.high);
			threadData.remoteDataLines.setLow(jniDataLines, line, dataPoint.low);
			threadData.remoteDataLines.setClose(jniDataLines, line, dataPoint.close);

			for (::jni::jint column = 0; column < optionalColumnCount; ++column)
			{
				assert(dataPoint.optionalColumns != nullptr);
				threadData.remoteDataLines.setOther(jniDataLines, line, column, dataPoint.optionalColumns[column]);
			}
		}

		threadData.remoteMainAppData.put(mainAppDataObject, jniAssetName, jniDataLines);
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

EGenotickResult CGenotick::StartInternal(TGenotickSessionId sessionId, const TGenotickStartArgs* pArgs) const
{
	if (pArgs == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const SThreadData& threadData = GetThreadData();
		const ::jni::jsize count = pArgs->elementCount;
		::jni::StringArray args = ::jni::StringArray::New(threadData.javaEnv, count, *threadData.remoteString.get());
		for (::jni::jsize index = 0; index < count; ++index)
		{
			const char* parameter = pArgs->elements[index];
			::jni::String jniParameter = ::jni::Make<::jni::String>(threadData.javaEnv, parameter);
			args.Set(threadData.javaEnv, index, jniParameter);
		}
		const remote::CErrorCode::TObject jniErrorCode = threadData.remoteMainInterface.start(sessionId, args);
		return remote::ErrorCodeToGenotickResult(threadData.remoteErrorCode, jniErrorCode);
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

EGenotickResult CGenotick::GetTimePointsInternal(TGenotickSessionId sessionId, IGenotickTimePoints** ppTimePoints) const
{
	if (ppTimePoints == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const SThreadData& threadData = GetThreadData();
		const remote::CTimePoints::TObject jniTimePoints = threadData.remoteMainInterface.getTimePoints(sessionId);
		if (jniTimePoints.Get() == nullptr)
			return EGenotickResult::ErrorInvalidSession;

		*ppTimePoints = new CGenotickTimePoints(jniTimePoints, threadData.remoteTimePoints, threadData.remoteTimePoint);
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

EGenotickResult CGenotick::GetPredictionsInternal(TGenotickSessionId sessionId, const char* assetName, IGenotickPredictions** ppPredictions) const
{
	if (ppPredictions == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const SThreadData& threadData = GetThreadData();
		const ::jni::String jniAssetName = ::jni::Make<::jni::String>(threadData.javaEnv, assetName);
		const remote::CPredictions::TObject jniPredictions = threadData.remoteMainInterface.getPredictions(sessionId, jniAssetName);
		if (jniPredictions.Get() == nullptr)
			return EGenotickResult::ErrorInvalidSession;

		*ppPredictions = new CGenotickPredictions(jniPredictions, threadData.remotePredictions, threadData.remotePrediction);
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

EGenotickResult CGenotick::GetNewestTimePointInternal(TGenotickSessionId sessionId, TGenotickTimePoint* pTimePoint) const
{
	if (pTimePoint == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const SThreadData& threadData = GetThreadData();
		const remote::CTimePoint::TObject jniTimePoint = threadData.remoteMainInterface.getNewestTimePoint(sessionId);
		if (jniTimePoint.Get() == nullptr)
			return EGenotickResult::ErrorInvalidSession;

		*pTimePoint = threadData.remoteTimePoint.getValue(jniTimePoint);
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

EGenotickResult CGenotick::GetNewestPredictionInternal(TGenotickSessionId sessionId, const char* assetName, EGenotickPrediction* pPrediction) const
{
	if (pPrediction == nullptr)
		return EGenotickResult::InvalidArgument;

	try
	{
		const SThreadData& threadData = GetThreadData();
		const ::jni::String jniAssetName = ::jni::Make<::jni::String>(threadData.javaEnv, assetName);
		const remote::CPrediction::TObject jniPrediction = threadData.remoteMainInterface.getNewestPrediction(sessionId, jniAssetName);
		if (jniPrediction.Get() == nullptr)
			return EGenotickResult::ErrorInvalidSession;

		const ::jni::jint predictionValue = threadData.remotePrediction.getValue(jniPrediction);
		*pPrediction = EGenotickPrediction::get_by_value(predictionValue);
		return EGenotickResult::Success;
	}
	catch (const ::jni::PendingJavaException& exception)
	{
		return HandleJavaException(GetThreadData().javaEnv, exception);
	}
	catch (const UnrecognizedThreadException& exception)
	{
		return HandleUnrecognizedThreadException(exception);
	}
}

EGenotickResult CGenotick::AttachCurrentThreadInternal(TGenotickBoolean asDaemon)
{
	if (HasThreadData())
	{
		return EGenotickResult::ThreadAlreadyAttached;
	}

	JNIEnv* pJavaEnv = nullptr;
	const jint jniResult = asDaemon
		? m_javaVM.AttachCurrentThreadAsDaemon(reinterpret_cast<void**>(&pJavaEnv), nullptr)
		: m_javaVM.AttachCurrentThread(reinterpret_cast<void**>(&pJavaEnv), nullptr);
	
	const EGenotickResult result = JniErrorToGenotickResult(jniResult);

	if (result == EGenotickResult::Success)
	{
		assert(pJavaEnv != nullptr);
		AddThreadData(*pJavaEnv);
	}
	return result;
}

EGenotickResult CGenotick::DetachCurrentThreadInternal()
{
	if (!HasThreadData())
	{
		return EGenotickResult::ThreadNotAttached;
	}

	RemoveThreadData();

	const jint jniResult = m_javaVM.DetachCurrentThread();
	return JniErrorToGenotickResult(jniResult);
}

EGenotickResult CGenotick::ReleaseInternal() const
{
	return m_loader.ReleaseInstanceFor(m_javaVM);
}

void CGenotick::AddThreadData(JNIEnv& javaEnv)
{
	m_threadDataMap.insert({std::this_thread::get_id(), SThreadData(javaEnv)});
}

void CGenotick::RemoveThreadData()
{
	m_threadDataMap.erase(std::this_thread::get_id());
}

bool CGenotick::HasThreadData() const
{
	TThreadDataMap::const_iterator it = m_threadDataMap.find(std::this_thread::get_id());
	return it != m_threadDataMap.end();
}

const CGenotick::SThreadData& CGenotick::GetThreadData() const
{
	try
	{
		return m_threadDataMap.at(std::this_thread::get_id());
	}
	catch (std::out_of_range&)
	{
		throw UnrecognizedThreadException("This thread is not attached to the Java VM");
	}
}

} // namespace jni
} // namespace genotick
