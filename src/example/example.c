
#include <igenotick.h>
#include <igenotick_utils.h>

#pragma comment(lib, "genotickcpp.lib")

#define GENOTICK_OUTDIR "D:\\Trading\\Code\\genotick\\.jar\\OUTPUT"
#define GENOTICK_DATADIR "D:\\Trading\\Code\\genotick\\.jar\\spx_data"
#define JAVA_CLASS_PATH "D:\\Trading\\Code\\genotick\\.jar\\genotick.jar"
#define JVM_PATH "C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	IGenotick* pInstance = 0;
	TGenotickJvmSettings jvmSettings = { 0 };
	jvmSettings.utf8_jvmDllPath = JVM_PATH;
	jvmSettings.utf8_javaClassPath = JAVA_CLASS_PATH;

	EGenotickResult result = LoadGenotick(&pInstance, &jvmSettings);

	if (result == GenotickResult_Success)
	{
		TGenotickInt32 version = pInstance->GetInterfaceVersion(pInstance);

		TGenotickInt32 sessionId = 0;
		TGenotickMainSettings mainSettings = { 0 };
		char buffer1[260] = { 0 };
		char buffer2[260] = { 0 };
		GenotickSetString(&mainSettings.populationDAO, buffer1, sizeof(buffer1));
		GenotickSetString(&mainSettings.dataDirectory, buffer2, sizeof(buffer2));

		result = pInstance->GetSettings(pInstance, sessionId, &mainSettings);

		mainSettings.startTimePoint = 20130101;
		mainSettings.endTimePoint = 20150101;
		GenotickSetConstString(&mainSettings.dataDirectory, GENOTICK_DATADIR);

		result = pInstance->ChangeSettings(pInstance, sessionId, &mainSettings);

		const char* arguments[] =
		{
			"input=external",
			"outdir=" GENOTICK_OUTDIR,
		};
		TGenotickStartArgs startArgs = { 0 };
		startArgs.elements = arguments;
		startArgs.elementCount = GENOTICK_ARRAY_SIZE(arguments);

		result = pInstance->Start(pInstance, sessionId, &startArgs);

		GENOTICK_SAFE_RELEASE(pInstance);
	}

	return 0;
}
