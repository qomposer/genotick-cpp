
#include <default.c>
#include <contract.c>
#define ZORRO_LITE_C
#include <genotick/igenotick.h>
#include <genotick/igenotick_utils.h>

#define GENOTICK_OUTDIR "include\\genotick\\.outdir"
#define GENOTICK_DATADIR ""
#define JAVA_CLASS_PATH "include\\genotick\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

EGenotickResult       g_result;
IGenotick*            g_pInstance;
IGenotickTimePoints*  g_pTimePoints;
IGenotickPredictions* g_pPredictions;
TGenotickTimePoint    g_timePoint;
EGenotickPrediction   g_prediction;
EGenotickPrediction   g_prevPrediction;
TGenotickAssetData    g_assetData;

void BuildTimePoint(TGenotickTimePoint* timePoint, int ibar)
{
	DATE date = wdateBar(ibar);
	long yyyymmdd = ymd(date);
	// date -= floor(date);
	// long h = (long)round(date * 24) % 24;
	// long m = (long)round(date * 24 * 60) % 60;
	// long s = (long)round(date * 24 * 60 * 60) % 60;
	// long dd = yyyymmdd % 100;
	// long mm = (yyyymmdd / 100) % 100;
	// long yyyy = (yyyymmdd / 10000);
	GenotickSetInt64(timePoint, 0, yyyymmdd);
}

void FillDataPoints(TGenotickDataPoint* dataPoints, int barCount)
{
	set(PEEK);
	
	int ibar;
	for (ibar = 0; ibar < barCount; ++ibar)
	{
		BuildTimePoint(&dataPoints[ibar].time, ibar);
		dataPoints[ibar].open = priceOpen(-ibar);
		dataPoints[ibar].high = priceHigh(-ibar);
		dataPoints[ibar].low = priceLow(-ibar);
		dataPoints[ibar].close = priceClose(-ibar);
	}
	
	reset(PEEK);
}


void CleanUpDataPoints(TGenotickDataPoint** p)
{
	if ((*p) != NULL)
	{
		free(*p);
		(*p) = NULL;
	}
}

void CleanUpTimePoints(IGenotickTimePoints** p)
{
	if ((*p) != NULL)
	{
		(*p)->Release(*p);
		(*p) = NULL;
	}
}

void CleanUpPredictions(IGenotickPredictions** p)
{
	if ((*p) != NULL)
	{
		(*p)->Release(*p);
		(*p) = NULL;
	}
}

void CleanUp()
{
	printf("\nCleanUp");
	CleanUpDataPoints(&g_assetData.dataPoints);
	CleanUpTimePoints(&g_pTimePoints);
	CleanUpPredictions(&g_pPredictions);
}

void InitGenotick()
{
	GenotickCreate = DefineApi("include/genotick/genotickcpp.dll!GenotickCreate");
	GenotickGetInstances = DefineApi("include/genotick/genotickcpp.dll!GenotickGetInstances");
	GenotickSpawnConsole = DefineApi("include/genotick/genotickcpp.dll!GenotickSpawnConsole");
	
	GenotickSpawnConsole();
	
	TGenotickCreationSettings creationSettings;
	GENOTICK_ZERO_STRUCT(creationSettings);
	creationSettings.utf8jvmDllPath = JVM_PATH;
	creationSettings.javaClassPath = JAVA_CLASS_PATH;
	
	g_result = GenotickGetOrCreate(&g_pInstance, &creationSettings);
	printf("\nGenotickGetOrCreate Error: %d", g_result);

	TGenotickInt32 version = g_pInstance->GetInterfaceVersion(g_pInstance);
	printf("\nGetInterfaceVersion Error: %d", version);
}

TGenotickDataPoint* AllocateDataPoints(int size)
{
	TGenotickDataPoint* dataPoints = malloc(sizeof(TGenotickDataPoint) * size);
	GENOTICK_ZERO_ARRAY(dataPoints, size, TGenotickDataPoint);
	return dataPoints;
}

void RunGenotick(TGenotickInt32 sessionId)
{
	g_result = g_pInstance->RemoveSession(g_pInstance, sessionId);
	printf("\nRemoveSession Error: %d", g_result);
	
	g_result = g_pInstance->CreateSession(g_pInstance, sessionId);
	printf("\nCreateSession Error: %d", g_result);
	
	TGenotickMainSettings mainSettings;
	GENOTICK_ZERO_STRUCT(mainSettings);
	char buffer1[260];
	char buffer2[260];
	GenotickSetString(&mainSettings.populationDAO, buffer1, 260);
	GenotickSetString(&mainSettings.dataDirectory, buffer2, 260);

	g_result = g_pInstance->GetSettings(g_pInstance, sessionId, &mainSettings);
	printf("\nGetSettings Error: %d", g_result);
	
	GenotickSetInt64(&mainSettings.startTimePoint, 0, 0);
	GenotickSetInt64(&mainSettings.endTimePoint, 0x7fffffff, 0xffffffff);
	GenotickSetConstString(&mainSettings.dataDirectory, GENOTICK_DATADIR);
	GenotickSetInt64(&mainSettings.randomSeed, 0, 1234567); // Adds reproducability

	g_result = g_pInstance->ChangeSettings(g_pInstance, sessionId, &mainSettings);
	printf("\nChangeSettings Error: %d", g_result);
	
	CleanUpDataPoints(&g_assetData.dataPoints);
	
	printf("\nNumBars: %d", NumBars);
	GENOTICK_ZERO_STRUCT(g_assetData);
	g_assetData.assetName = Asset;
	g_assetData.dataPoints = AllocateDataPoints(NumBars);
	g_assetData.dataPointCount = NumBars;
	g_assetData.firstDataPointIsNewest = GenotickFalse;
	
	FillDataPoints(g_assetData.dataPoints, g_assetData.dataPointCount);
	
	g_result = g_pInstance->SetAssetData(g_pInstance, sessionId, &g_assetData);
	printf("\nSetAssetData Error: %d", g_result);
	
	const char* arguments[2];
	arguments[0] = "input=external";
	arguments[1] = strf("outdir=%s", GENOTICK_OUTDIR);
	TGenotickStartArgs startArgs;
	GENOTICK_ZERO_STRUCT(startArgs);
	startArgs.elements = arguments;
	startArgs.elementCount = 2;

	g_result = g_pInstance->Start(g_pInstance, sessionId, &startArgs);
	printf("\nStart Error: %d", g_result);

	g_result = g_pInstance->GetTimePoints(g_pInstance, sessionId, &g_pTimePoints);
	printf("\nGetTimePoints Error: %d", g_result);
		
	g_result = g_pInstance->GetPredictions(g_pInstance, sessionId, g_assetData.assetName, &g_pPredictions);
	printf("\nGetPredictions Error: %d", g_result);
	if (g_pPredictions != NULL)
		printf("\nPredictions Count: %d", g_pPredictions->Size(g_pPredictions));
	
	g_result = g_pInstance->GetNewestTimePoint(g_pInstance, sessionId, &g_timePoint);
	printf("\nGetNewestTimePoint Error: %d", g_result);
	
	g_result = g_pInstance->GetNewestPrediction(g_pInstance, sessionId, g_assetData.assetName, &g_prediction);
	printf("\nGetNewestPrediction Error: %d", g_result);
	
	g_result = g_pInstance->RemoveSession(g_pInstance, sessionId);
	printf("\nRemoveSession Error: %d", g_result);
}

void TradePrediction()
{
	if (g_pPredictions != NULL)
	{
		const EGenotickPrediction* pPrediction = g_pPredictions->Get(g_pPredictions, Bar-1);
		const EGenotickPrediction prediction = *pPrediction;
		printf("\nBar %d of %d: Prediction: %d", Bar, NumBars, prediction);
		if (prediction != g_prevPrediction)
		{
			switch (prediction)
			{
				case GenotickPrediction_Out: exitLong(); exitShort(); break;
				case GenotickPrediction_Up: enterLong(); break;
				case GenotickPrediction_Down: enterShort(); break;
			}
			g_prevPrediction = prediction;
		}
	}
}

void main()
{
	set(OPENEND);
	set(TICKS);
	BarPeriod = 1440;
	NumYears = 1;
	
	g_result = 0;
	g_pInstance = 0;
	g_pTimePoints = NULL;
	g_pPredictions = NULL;
	GENOTICK_ZERO_STRUCT(g_timePoint);
	g_prediction = GenotickPrediction_Out;
	g_prevPrediction = GenotickPrediction_Out;
	GENOTICK_ZERO_STRUCT(g_assetData);

	InitGenotick();
}

void run()
{	
	if (is(FIRSTRUN))
	{
		RunGenotick(0);
	}
	
	TradePrediction();
	
	if (is(EXITRUN))
	{
		CleanUp();
	}
}
