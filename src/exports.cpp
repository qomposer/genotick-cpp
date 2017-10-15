
#include <genotick/loader.h>

::genotick::CLoader g_genotickLoader;

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
