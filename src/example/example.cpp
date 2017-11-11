
#pragma comment(lib, "genotickcpplib.lib")

#include <igenotick.h>
#include <igenotick_utils.h>
#include <cassert>
#include <thread>
#include <vector>
#include <time_counter.h>

#define GENOTICK_OUTDIR "..\\jar\\.outdir"
#define GENOTICK_DATADIR ""
#define JAVA_CLASS_PATH "..\\jar\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

enum ETestRunFlag
{
	eTestRunFlag_None        = 0,
	eTestRunFlag_IsNewThread = 1<<0,
	eTestRunFlag_UseOutput   = 1<<1,
};

void GenerateDataPoints(TGenotickDataPoint* dataPoints, TGenotickSize size)
{
	for (TGenotickSize i = 0; i < size; ++i)
	{
		dataPoints[i].time   = 20130101 + i;
		dataPoints[i].open   = i;
		dataPoints[i].high   = i + 0.5;
		dataPoints[i].low    = i - 0.5;
		dataPoints[i].close  = i + 0.2;
	}
}

void GenotickTestRun(IGenotick* pInstance, ETestRunFlag flags)
{
	EGenotickResult result;

	if (flags & eTestRunFlag_IsNewThread)
	{
		result = pInstance->AttachCurrentThread(false);
		assert(result == EGenotickResult::Success);
	}

	TGenotickInt32 version = pInstance->GetInterfaceVersion();
	assert(version == GENOTICK_INTERFACE_VERSION);

	TGenotickInt32 sessionId = 0;
	result = pInstance->CreateSession(sessionId);
	assert(result == EGenotickResult::Success);

	TGenotickMainSettings mainSettings;
	char buffer1[260] = { 0 };
	char buffer2[260] = { 0 };
	GenotickSetString(mainSettings.populationDAO, buffer1);
	GenotickSetString(mainSettings.dataDirectory, buffer2);

	result = pInstance->GetSettings(sessionId, &mainSettings);
	assert(result == EGenotickResult::Success);

	mainSettings.startTimePoint = 20130101;
	mainSettings.endTimePoint = 20150101;
	GenotickSetConstString(mainSettings.dataDirectory, GENOTICK_DATADIR);

	result = pInstance->ChangeSettings(sessionId, &mainSettings);
	assert(result == EGenotickResult::Success);

	TGenotickDataPoint dataPoints[200];
	TGenotickAssetData assetData;
	assetData.assetName = "ASSET";
	assetData.dataPoints = dataPoints;
	assetData.dataPointCount = GenotickArraySize(dataPoints);
	assetData.firstDataPointIsNewest = GenotickFalse;
	GenerateDataPoints(dataPoints, assetData.dataPointCount);

	result = pInstance->SetAssetData(sessionId, &assetData);
	assert(result == EGenotickResult::Success);

	const char* arguments[] =
	{
		"input=external",
		"outdir=" GENOTICK_OUTDIR,
		(flags & eTestRunFlag_UseOutput) ? "output=console" : "output=none"
	};
	TGenotickStartArgs startArgs;
	startArgs.elements = arguments;
	startArgs.elementCount = GenotickArraySize(arguments);

	result = pInstance->Start(sessionId, &startArgs);
	assert(result == EGenotickResult::Success);

	IGenotickTimePointsPtr pTimePoints;
	IGenotickPredictionsPtr pPredictions;
	TGenotickTimePoint timePoint = 0;
	EGenotickPrediction prediction = EGenotickPrediction::Out;

	result = GenotickGetTimePoints(*pInstance, sessionId, pTimePoints);
	assert(result == EGenotickResult::Success);

	result = GenotickGetPredictions(*pInstance, sessionId, assetData.assetName, pPredictions);
	assert(result == EGenotickResult::Success);

	result = pInstance->GetNewestTimePoint(sessionId, &timePoint);
	assert(result == EGenotickResult::Success);

	result = pInstance->GetNewestPrediction(sessionId, assetData.assetName, &prediction);
	assert(result == EGenotickResult::Success);

	result = pInstance->RemoveSession(sessionId);
	assert(result == EGenotickResult::Success);

	for (TGenotickSize i = 0, c = pTimePoints->GetElementCount(); i < c; ++i)
	{
		const TGenotickTimePoint* pTimePoint = pTimePoints->GetElement(i);
		(void)pTimePoint;
	}

	for (TGenotickSize i = 0, c = pPredictions->GetElementCount(); i < c; ++i)
	{
		const EGenotickPrediction* pPrediction = pPredictions->GetElement(i);
		(void)pPrediction;
	}
	
	if (flags & eTestRunFlag_IsNewThread)
	{
		result = pInstance->DetachCurrentThread();
		assert(result == EGenotickResult::Success);
	}
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	IGenotick* pInstance = nullptr;

	TGenotickCreationSettings creationSettings;
	creationSettings.utf8_jvmDllPath = JVM_PATH;
	creationSettings.javaClassPath = JAVA_CLASS_PATH;
	//creationSettings.javaDebugAddress = "127.0.0.1:8888";

	EGenotickResult result = GenotickCreate(&pInstance, &creationSettings);
	assert(result == EGenotickResult::Success);

	CTimeCounter<std::chrono::nanoseconds, CTimeCounterStdOutPrinter> counter;

	for (int run = 0; run < 2; ++run)
	{
		result = GenotickGetOrCreate(&pInstance, &creationSettings);
		assert(result == EGenotickResult::Success);

		GenotickTestRun(pInstance, eTestRunFlag_UseOutput);
	}

	counter.restart();

	std::vector<std::thread> threads;
	for (int thread = 0; thread < 8; ++thread)
	{
		threads.push_back(std::thread(GenotickTestRun, pInstance, eTestRunFlag_IsNewThread));
	}
	for (std::thread& thread : threads)
	{
		thread.join();
	}

	counter.stop();

	GenotickSafeRelease(pInstance);

	return 0;
}
