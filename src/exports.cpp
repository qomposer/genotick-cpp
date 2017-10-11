
#include <genotick/loader.h>

CLoader g_genotickLoader;

EGenotickResult GENOTICK_CALL LoadGenotick(IGenotick** ppInstance, const TGenotickLoadSettings* pSettings)
{
	return g_genotickLoader.LoadGenotick(ppInstance, pSettings);
}
