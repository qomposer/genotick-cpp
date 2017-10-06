
#include <igenotick.h>

#pragma comment(lib, "genotickcpp.lib")

#define GENOTICK_OUTDIR "D:\\Trading\\Code\\genotick\\.jar\\OUTPUT"
#define GENOTICK_DATADIR "D:\\Trading\\Code\\genotick\\.jar\\spx_data"
#define JAVA_CLASS_PATH "D:\\Trading\\Code\\genotick\\.jar\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

void SetString(TGenotickString* dst, char* src, unsigned int capacity)
{
	dst->utf8_buffer = src;
	dst->capacity = capacity;
}

void SetConstString(TGenotickString* dst, const char* src)
{
	dst->utf8_buffer = (char*)src;
	dst->capacity = 0;
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	IGenotick* pInstance = 0;
	SGenotickJvmSettings jvmSettings = { 0 };
	jvmSettings.utf8_jvmDllPath = JVM_PATH;
	jvmSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

	EGenotickResult result = LoadGenotick(&pInstance, &jvmSettings);

	if (result == eGenotickResult_Success)
	{
		TGenotickInt32 version = pInstance->GetInterfaceVersion(pInstance);

		SGenotickMainSettings mainSettings = { 0 };
		char buffer1[260] = { 0 };
		char buffer2[260] = { 0 };
		SetString(&mainSettings.populationDAO, buffer1, sizeof(buffer1));
		SetString(&mainSettings.dataDirectory, buffer2, sizeof(buffer2));

		result = pInstance->GetSettings(pInstance, &mainSettings);

		mainSettings.startTimePoint = 20130101;
		mainSettings.endTimePoint = 20150101;
		SetConstString(&mainSettings.dataDirectory, GENOTICK_DATADIR);

		result = pInstance->ChangeSettings(pInstance, &mainSettings);

		const char* arguments[] =
		{
			"input=external",
			"outdir=" GENOTICK_OUTDIR,
		};
		SGenotickStartSettings startSettings = { 0 };
		startSettings.parameters = arguments;
		startSettings.parameterCount = ARRAY_SIZE(arguments);

		result = pInstance->Start(pInstance, &startSettings);

		GENOTICK_SAFE_RELEASE(pInstance);
	}

	return 0;
}
