
#include <default.c>
#define ZORRO_LITE_C
#include <genotick/igenotick.h>
#include <genotick/igenotick_utils.h>

#define GENOTICK_OUTDIR "include\\genotick\\.outdir"
#define GENOTICK_DATADIR "include\\genotick\\.data"
#define JAVA_CLASS_PATH "include\\genotick\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

void GenerateDataPoints(TGenotickDataPoint* dataPoints, TGenotickSize size)
{
	TGenotickSize i;
	for (i = 0; i < size; ++i)
	{		
		GenotickSetTimePoint(&dataPoints[i].time, 0, 20130101 + i);
		dataPoints[i].open   = size*3 + i * 1.0;
		dataPoints[i].high   = size*3 + i * 1.0 + 0.5;
		dataPoints[i].low    = size*3 + i * 1.0 - 0.5;
		dataPoints[i].close  = size*3 + i * 1.0 + 0.2;
		dataPoints[i].volume = 10000.0;
	}
}

void main()
{	
	GenotickCreate = DefineApi("include/genotick/genotickcpp.dll!GenotickCreate");
	GenotickGetInstances = DefineApi("include/genotick/genotickcpp.dll!GenotickGetInstances");
		
	EGenotickResult result = 0;
	
	IGenotick* pInstance = 0;
	TGenotickCreationSettings creationSettings;
	GENOTICK_ZERO_STRUCT(creationSettings);
	creationSettings.utf8_jvmDllPath = JVM_PATH;
	creationSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

	result = GenotickGetOrCreate(&pInstance, &creationSettings);
	printf("\nGenotickGetOrCreate: %d", result);

	TGenotickInt32 version = pInstance->GetInterfaceVersion(pInstance);
	printf("\nGetInterfaceVersion: %d", version);
	
	result = pInstance->RemoveAllSessions(pInstance);
	printf("\nRemoveAllSessions: %d", result);
	
	TGenotickInt32 sessionId = 0;
	result = pInstance->CreateSession(pInstance, sessionId);
	printf("\nCreateSession: %d", result);

	TGenotickMainSettings mainSettings;
	GENOTICK_ZERO_STRUCT(mainSettings);
	char buffer1[260];
	char buffer2[260];
	GenotickSetString(&mainSettings.populationDAO, buffer1, sizeof(buffer1));
	GenotickSetString(&mainSettings.dataDirectory, buffer2, sizeof(buffer2));

	result = pInstance->GetSettings(pInstance, sessionId, &mainSettings);
	printf("\nGetSettings: %d", result);
	
	GenotickSetTimePoint(&mainSettings.startTimePoint, 0, 20130101);
	GenotickSetTimePoint(&mainSettings.endTimePoint, 0, 20150101);
	GenotickSetConstString(&mainSettings.dataDirectory, GENOTICK_DATADIR);

	result = pInstance->ChangeSettings(pInstance, sessionId, &mainSettings);
	printf("\nChangeSettings: %d", result);
	
	TGenotickDataPoint dataPoints[200];
	GENOTICK_ZERO_ARRAY(dataPoints, 200, TGenotickDataPoint);
	TGenotickAssetData assetData;
	GENOTICK_ZERO_STRUCT(assetData);
	assetData.assetName = "ASSET";
	assetData.dataPoints = dataPoints;
	assetData.dataPointCount = 200;
	assetData.firstDataPointIsNewest = GenotickFalse;
	GenerateDataPoints(dataPoints, assetData.dataPointCount);

	result = pInstance->SetAssetData(pInstance, sessionId, &assetData);
	printf("\nSetAssetData: %d", result);

	const char* arguments[2];
	arguments[0] = "input=external";
	arguments[1] = strf("outdir=%s", GENOTICK_OUTDIR);
	TGenotickStartArgs startArgs;
	GENOTICK_ZERO_STRUCT(startArgs);
	startArgs.elements = arguments;
	startArgs.elementCount = 2;

	result = pInstance->Start(pInstance, sessionId, &startArgs);
	printf("\nStart: %d", result);

	IGenotickTimePoints* pTimePoints;
	IGenotickPredictions* pPredictions;
	TGenotickTimePoint timePoint;
	EGenotickPrediction prediction;
	
	result = pInstance->GetTimePoints(pInstance, sessionId, &pTimePoints);
	printf("\nGetTimePoints: %d", result);
	
	result = pInstance->GetPredictions(pInstance, sessionId, "ASSET", &pPredictions);
	printf("\nGetPredictions: %d", result);
	
	result = pInstance->GetNewestTimePoint(pInstance, sessionId, &timePoint);
	printf("\nGetNewestTimePoint: %d", result);
	
	result = pInstance->GetNewestPrediction(pInstance, sessionId, "ASSET", &prediction);
	printf("\nGetNewestPrediction: %d", result);
	
	result = pInstance->RemoveSession(pInstance, sessionId);
	printf("\nRemoveSession: %d", result);
	
	pTimePoints->Release(pTimePoints);
	pPredictions->Release(pPredictions);
}

void run()
{

}

