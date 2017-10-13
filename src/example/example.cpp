
#include <igenotick.h>
#include <igenotick_utils.h>

#pragma comment(lib, "genotickcpp.lib")

#define GENOTICK_OUTDIR "D:\\Trading\\Code\\genotick\\.jar\\OUTPUT"
#define GENOTICK_DATADIR "D:\\Trading\\Code\\genotick\\.jar\\spx_data"
#define JAVA_CLASS_PATH "D:\\Trading\\Code\\genotick\\.jar\\genotick.jar"
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
	TGenotickLoadSettings jvmSettings;
	jvmSettings.utf8_jvmDllPath = JVM_PATH;
	jvmSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

	result = LoadGenotick(&pInstance, &jvmSettings);

	IGenotickList* pInstances = nullptr;

	result = GetGenotickInstances(&pInstances, &jvmSettings);

	GenotickSafeRelease(pInstances);

	if (result == EGenotickResult::Success)
	{
		TGenotickInt32 version = pInstance->GetInterfaceVersion();

		TGenotickInt32 sessionId = 0;
		result = pInstance->CreateSession(sessionId);

		TGenotickMainSettings mainSettings;
		char buffer1[260] = { 0 };
		char buffer2[260] = { 0 };
		GenotickSetString(mainSettings.populationDAO, buffer1);
		GenotickSetString(mainSettings.dataDirectory, buffer2);

		result = pInstance->GetSettings(sessionId, &mainSettings);

		mainSettings.startTimePoint = 20130101;
		mainSettings.endTimePoint = 20150101;
		GenotickSetConstString(mainSettings.dataDirectory, GENOTICK_DATADIR);

		result = pInstance->ChangeSettings(sessionId, &mainSettings);

		TGenotickDataPoint dataPoints[200];
		TGenotickAssetData assetData;
		assetData.assetName = "ASSET";
		assetData.dataPoints = dataPoints;
		assetData.dataPointCount = GenotickArraySize(dataPoints);
		assetData.firstDataPointIsNewest = GenotickFalse;
		GenerateDataPoints(dataPoints, assetData.dataPointCount);

		result = pInstance->SetAssetData(sessionId, &assetData);

		const char* arguments[] =
		{
			"input=external",
			"outdir=" GENOTICK_OUTDIR,
		};
		TGenotickStartArgs startArgs;
		startArgs.elements = arguments;
		startArgs.elementCount = GenotickArraySize(arguments);

		result = pInstance->Start(sessionId, &startArgs);

		IGenotickTimePoints* pTimePoints = nullptr;
		IGenotickPredictions* pPredictions = nullptr;
		TGenotickTimePoint timePoint = 0;
		EGenotickPrediction prediction = EGenotickPrediction::Out;
		result = pInstance->GetTimePoints(sessionId, &pTimePoints);
		result = pInstance->GetPredictions(sessionId, "ASSET", &pPredictions);
		result = pInstance->GetNewestTimePoint(sessionId, &timePoint);
		result = pInstance->GetNewestPrediction(sessionId, "ASSET", &prediction);

		result = pInstance->RemoveSession(sessionId);

		GenotickSafeRelease(pTimePoints);
		GenotickSafeRelease(pPredictions);
		GenotickSafeRelease(pInstance);
	}

	return 0;
}
