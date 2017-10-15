
#include "console.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

bool CConsole::Create()
{
	if (::AllocConsole() == TRUE)
	{
		::SetConsoleTitleA("Genotick Console");

		*(iobuf*)stdout = *(iobuf*)::_fdopen(::_open_osfhandle((long)::GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");
		*(iobuf*)stderr = *(iobuf*)::_fdopen(::_open_osfhandle((long)::GetStdHandle(STD_ERROR_HANDLE), _O_TEXT), "w");
		*(iobuf*)stdin = *(iobuf*)::_fdopen(::_open_osfhandle((long)::GetStdHandle(STD_INPUT_HANDLE), _O_TEXT), "r");
		::setvbuf(stdout, NULL, _IONBF, 0);
		::setvbuf(stderr, NULL, _IONBF, 0);
		::setvbuf(stdin, NULL, _IONBF, 0);

		::std::cout << "Spawned console" << ::std::endl;
		return true;
	}

	return false;
}
