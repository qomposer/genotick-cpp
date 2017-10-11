
#include <genotick/loader.h>

CJavaLoader g_javaLoader;

EGenotickResult GENOTICK_CALL LoadGenotick(IGenotick** ppInstance, const TGenotickJvmSettings* pSettings)
{
	return g_javaLoader.LoadGenotick(ppInstance, pSettings);
}
