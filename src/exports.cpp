
#include <genotick/loader.h>

::genotick::CLoader g_genotickLoader;

EGenotickResult GENOTICK_CALL LoadGenotick(IGenotick** ppInstance, const TGenotickLoadSettings* pSettings)
{
	return g_genotickLoader.LoadGenotick(ppInstance, pSettings);
}
