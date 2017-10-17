
#include <igenotick.h>
#include <igenotick_utils.h>
#include <cassert>

#pragma comment(lib, "genotickcpp.lib")

#define GENOTICK_OUTDIR "..\\jar\\.outdir"
#define GENOTICK_DATADIR ""
#define JAVA_CLASS_PATH "..\\jar\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

void GenerateDataPoints(TGenotickDataPoint* dataPoints, TGenotickSize size)
{
	for (TGenotickSize i = 0; i < size; ++i)
	{
		dataPoints[i].time   = 20130101 + i;
		dataPoints[i].open   = size*3 + i * 1.0;
		dataPoints[i].high   = size*3 + i * 1.0 + 0.5;
		dataPoints[i].low    = size*3 + i * 1.0 - 0.5;
		dataPoints[i].close  = size*3 + i * 1.0 + 0.2;
		dataPoints[i].volume = 10000.0;
	}
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	EGenotickResult result;
	IGenotick* pInstance = nullptr;

	for (int run = 0; run < 2; ++run)
	{
		TGenotickCreationSettings creationSettings;
		creationSettings.utf8_jvmDllPath = JVM_PATH;
		creationSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

		result = GenotickGetOrCreate(&pInstance, &creationSettings);
		assert(result == EGenotickResult::Success);

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
		};
		TGenotickStartArgs startArgs;
		startArgs.elements = arguments;
		startArgs.elementCount = GenotickArraySize(arguments);

		result = pInstance->Start(sessionId, &startArgs);
		assert(result == EGenotickResult::Success);

		IGenotickTimePoints* pTimePoints = nullptr;
		IGenotickPredictions* pPredictions = nullptr;
		TGenotickTimePoint timePoint = 0;
		EGenotickPrediction prediction = EGenotickPrediction::Out;

		result = pInstance->GetTimePoints(sessionId, &pTimePoints);
		assert(result == EGenotickResult::Success);

		result = pInstance->GetPredictions(sessionId, assetData.assetName, &pPredictions);
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

		GenotickSafeRelease(pTimePoints);
		GenotickSafeRelease(pPredictions);
	}

	GenotickSafeRelease(pInstance);

	return 0;
}
