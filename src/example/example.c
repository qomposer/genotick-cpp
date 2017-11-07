
#include <igenotick.h>
#include <igenotick_utils.h>
#include <assert.h>

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
		dataPoints[i].open   = i;
		dataPoints[i].high   = i + 0.5;
		dataPoints[i].low    = i - 0.5;
		dataPoints[i].close  = i + 0.2;
	}
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	EGenotickResult result;
	IGenotick* pInstance = 0;

	for (int run = 0; run < 2; ++run)
	{
		TGenotickCreationSettings creationSettings = { 0 };
		creationSettings.utf8_jvmDllPath = JVM_PATH;
		creationSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

		result = GenotickGetOrCreate(&pInstance, &creationSettings);
		assert(result == GenotickResult_Success);

		TGenotickInt32 version = pInstance->GetInterfaceVersion(pInstance);
		assert(version == GENOTICK_INTERFACE_VERSION);

		TGenotickInt32 sessionId = 0;
		result = pInstance->CreateSession(pInstance, sessionId);
		assert(result == GenotickResult_Success);

		TGenotickMainSettings mainSettings = { 0 };
		char buffer1[260] = { 0 };
		char buffer2[260] = { 0 };
		GenotickSetString(&mainSettings.populationDAO, buffer1, sizeof(buffer1));
		GenotickSetString(&mainSettings.dataDirectory, buffer2, sizeof(buffer2));

		result = pInstance->GetSettings(pInstance, sessionId, &mainSettings);
		assert(result == GenotickResult_Success);

		mainSettings.startTimePoint = 20130101;
		mainSettings.endTimePoint = 20150101;
		GenotickSetConstString(&mainSettings.dataDirectory, GENOTICK_DATADIR);

		result = pInstance->ChangeSettings(pInstance, sessionId, &mainSettings);
		assert(result == GenotickResult_Success);

		TGenotickDataPoint dataPoints[200] = { 0 };
		TGenotickAssetData assetData = { 0 };
		assetData.assetName = "ASSET";
		assetData.dataPoints = dataPoints;
		assetData.dataPointCount = GENOTICK_ARRAY_SIZE(dataPoints);
		assetData.firstDataPointIsNewest = GenotickFalse;
		GenerateDataPoints(dataPoints, assetData.dataPointCount);

		result = pInstance->SetAssetData(pInstance, sessionId, &assetData);
		assert(result == GenotickResult_Success);

		const char* arguments[] =
		{
			"input=external",
			"outdir=" GENOTICK_OUTDIR,
		};
		TGenotickStartArgs startArgs = { 0 };
		startArgs.elements = arguments;
		startArgs.elementCount = GENOTICK_ARRAY_SIZE(arguments);

		result = pInstance->Start(pInstance, sessionId, &startArgs);
		assert(result == GenotickResult_Success);

		IGenotickTimePoints* pTimePoints = 0;
		IGenotickPredictions* pPredictions = 0;
		TGenotickTimePoint timePoint = 0;
		EGenotickPrediction prediction = GenotickPrediction_Out;

		result = pInstance->GetTimePoints(pInstance, sessionId, &pTimePoints);
		assert(result == GenotickResult_Success);

		result = pInstance->GetPredictions(pInstance, sessionId, assetData.assetName, &pPredictions);
		assert(result == GenotickResult_Success);

		result = pInstance->GetNewestTimePoint(pInstance, sessionId, &timePoint);
		assert(result == GenotickResult_Success);

		result = pInstance->GetNewestPrediction(pInstance, sessionId, assetData.assetName, &prediction);
		assert(result == GenotickResult_Success);

		result = pInstance->RemoveSession(pInstance, sessionId);
		assert(result == GenotickResult_Success);

		for (TGenotickSize i = 0, c = pTimePoints->GetElementCount(pTimePoints); i < c; ++i)
		{
			const TGenotickTimePoint* pTimePoint = pTimePoints->GetElement(pTimePoints, i);
			(void)pTimePoint;
		}

		for (TGenotickSize i = 0, c = pPredictions->GetElementCount(pPredictions); i < c; ++i)
		{
			const EGenotickPrediction* pPrediction = pPredictions->GetElement(pPredictions, i);
			(void)pPrediction;
		}

		GENOTICK_SAFE_RELEASE(pTimePoints);
		GENOTICK_SAFE_RELEASE(pPredictions);
	}

	GENOTICK_SAFE_RELEASE(pInstance);

	return 0;
}
