
#include <common/msvc_codecvt_fix_impl.h>
#include <genotick/jni/loader.h>

#ifdef GENOTICK_DLL

#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	(void)hinstDLL;
	(void)fdwReason;
	(void)lpvReserved;
	return TRUE;
}

#endif

::genotick::jni::CLoader g_genotickLoader;

EGenotickResult GENOTICK_CALL GenotickCreate(IGenotick** ppInstance, const TGenotickCreationSettings* pSettings)
{
	return g_genotickLoader.GenotickCreate(ppInstance, pSettings);
}

EGenotickResult GENOTICK_CALL GenotickGetInstances(IGenotickList** ppInstances, const TGenotickCreationSettings* pSettings)
{
	return g_genotickLoader.GenotickGetInstances(ppInstances, pSettings);
}

EGenotickResult GENOTICK_CALL GenotickSpawnConsole()
{
	return g_genotickLoader.GenotickSpawnConsole();
}

EGenotickResult GENOTICK_CALL GenotickDestroyConsole()
{
	return g_genotickLoader.GenotickDestroyConsole();
}
