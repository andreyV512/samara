// RunUSPC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "App.h"
#include "Registry.h"
#include "InitTcp.h"

int _tmain(int argc, _TCHAR* argv[])
{
	HWND wnd = GetConsoleWindow();
	GetSystemMenu( wnd, TRUE); //сбрасываем рабочие копии системного меню
	EnableMenuItem( GetSystemMenu(wnd, false), SC_CLOSE, MF_DISABLED|MF_GRAYED);

	InitTcp initTcp;

	App app;
	if(!app.Init()) 
	{
		getchar();
		app.Destroy();
		return 0;
	}
	
	RegistryPathApplications path;
	
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, path(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	
	WaitForSingleObject(pi.hProcess, INFINITE);
	
	app.Destroy();
	return 0;
}

