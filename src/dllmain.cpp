
#ifdef GENOTICK_DLL

#include <windows.h>

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	(void)hinstDLL;
	(void)fdwReason;
	(void)lpvReserved;
	return TRUE;
}

#endif
