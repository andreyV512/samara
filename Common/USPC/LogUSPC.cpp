#include "stdafx.h"
#include "LogUSPC.h"
#include <stdio.h>
#include "Registry.h"
#include "debug_tools/DebugMess.h"

#include "App/Config.h"
#ifdef DEBUG_ITEMS
wchar_t *path = L"D:\\Shared\\USPC7100log.txt";
#endif
#pragma warning(disable : 4996)
void LogUSPC::Clear()
{
#ifndef DEBUG_ITEMS
	RegistryPathLogFile t;
	wchar_t *path = t();
#endif
	FILE *f = _wfopen(path, L"wt");
	if(f)fclose(f);
}
void LogUSPC::Open()
{
	offs = 0;
#ifndef DEBUG_ITEMS
	RegistryPathLogFile t;
	wchar_t *path = t();
#endif
	char b[128];
	FILE *f = _wfopen(path, L"r");
	if(f)
	{
		int i = 0;
		while(true)
		{
			int k = i % 128;
			if(!fgets(b, 128, f))break;
			if(' ' == b[0])continue;
			strcpy(buffer[k], b);
			++i;
		}
		for(int k = i;k < 128; ++k) buffer[k][0] = '\0';
		if(i > 128) offs = i % 128;
		fclose(f);
	}
}
 char *LogUSPC::operator[](int i)
 {
	 int k = (offs + i) % 128;
	 return buffer[k];
 }
