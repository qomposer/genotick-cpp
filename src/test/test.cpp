
#include "igenotick.h"

#ifdef GENOTICK_LIB
#pragma comment(lib, "genotickcpp.lib")
#endif

#define GENOTICK_OUTDIR "D:\\Trading\\Code\\genotick\\.jar\\OUTPUT"
#define JAVA_CLASS_PATH "D:\\Trading\\Code\\genotick\\.jar\\genotick.jar"
#define JVM_PATH L"C:\\Program Files (x86)\\Java\\jre1.8.0_144\\bin\\client\\jvm.dll"

template<typename T, int size>
int GetArrayLength(T(&)[size])
{
	return size;
}

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	IGenotick* pInstance = nullptr;
	SGenotickJvmSettings jvmSettings = { 0 };
	jvmSettings.jvmDllPath = JVM_PATH;
	jvmSettings.javaClassPath = JAVA_CLASS_PATH;

	EGenotickResult result = LoadGenotick(&pInstance, &jvmSettings);

	if (result == eGenotickResult_Success)
	{
		const char* arguments[] =
		{
			"input=external",
			"outdir=" GENOTICK_OUTDIR,
		};

		EGenotickResult result;
		SGenotickMainSettings mainSettings = { 0 };
		SGenotickStartSettings startSettings = { 0 };
		startSettings.parameters = arguments;
		startSettings.parameterCount = GetArrayLength(arguments);

		TGenotickInt32 version = pInstance->GetInterfaceVersion();
		result = pInstance->GetSettings(&mainSettings);
		result = pInstance->ChangeSettings(&mainSettings);
		result = pInstance->Start(&startSettings);

		SAFE_RELEASE(pInstance);
	}

	return 0;
}
