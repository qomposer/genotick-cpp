
#pragma once

class CConsole
{
	struct iobuf
	{
		char *_ptr;
		int   _cnt;
		char *_base;
		int   _flag;
		int   _file;
		int   _charbuf;
		int   _bufsiz;
		char *_tmpfname;
	};

public:
	static bool Create();
};
