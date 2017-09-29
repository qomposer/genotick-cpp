
#include "igenotick.h"

#ifdef GENOTICK_LIB
#pragma comment(lib, "genotickcpp.lib")
#endif

#define GENOTICK_OUTDIR "D:\\Trading\\Code\\genotick\\.jar\\OUTPUT"
#define GENOTICK_DATADIR "D:\\Trading\\Code\\genotick\\.jar\\spx_data"
#define JAVA_CLASS_PATH "D:\\Trading\\Code\\genotick\\.jar\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

template<typename T, unsigned int size>
unsigned int GetArrayLength(T(&)[size])
{
	return size;
}

template<int size>
void SetString(TGenotickString& dst, char(&src)[size])
{
	dst.utf8_buffer = src;
	dst.capacity = size;
}

void SetString(TGenotickString& dst, char* src)
{
	dst.utf8_buffer = src;
	dst.capacity = 0;
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	IGenotick* pInstance = nullptr;
	SGenotickJvmSettings jvmSettings = { 0 };
	jvmSettings.utf8_jvmDllPath = JVM_PATH;
	jvmSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

	EGenotickResult result = LoadGenotick(&pInstance, &jvmSettings);

	if (result == EGenotickResult::Success)
	{
		TGenotickInt32 version = pInstance->GetInterfaceVersion();

		SGenotickMainSettings mainSettings = { 0 };
		char buffer1[260] = { 0 };
		char buffer2[260] = { 0 };
		SetString(mainSettings.populationDAO, buffer1);
		SetString(mainSettings.dataDirectory, buffer2);

		result = pInstance->GetSettings(&mainSettings);

		mainSettings.startTimePoint = 20130101;
		mainSettings.endTimePoint = 20150101;
		SetString(mainSettings.dataDirectory, GENOTICK_DATADIR);

		result = pInstance->ChangeSettings(&mainSettings);

		const char* arguments[] =
		{
			"input=external",
			"outdir=" GENOTICK_OUTDIR,
		};
		SGenotickStartSettings startSettings = { 0 };
		startSettings.parameters = arguments;
		startSettings.parameterCount = GetArrayLength(arguments);

		result = pInstance->Start(&startSettings);

		SAFE_RELEASE(pInstance);
	}

	return 0;
}
