
#include "java_loader.h"

CJavaLoader g_javaLoader;

EGenotickResult GENOTICK_CALL LoadGenotick(IGenotick** ppInstance, const SGenotickJvmSettings* pSettings)
{
	return g_javaLoader.LoadGenotick(ppInstance, pSettings);
}
