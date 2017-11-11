
#ifdef GENOTICK_DLL

#include <windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	(void)hinstDLL;
	(void)fdwReason;
	(void)lpvReserved;
	return TRUE;
}

#endif

#include "msvc_codecvt_fix_impl.h"
