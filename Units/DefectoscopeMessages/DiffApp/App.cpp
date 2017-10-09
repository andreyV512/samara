#include "StdAfx.h"
#include "App.h"
#include "Windows\MainWindow.h"
#include "window_tool\EmptyWindow.h"
#include "debug_tools/DebugMess.h"
#include "Log\LogBuffer.h"
#include "Ini\WindowsPosition.h"

namespace{
	MainWindow mainWindow;
}

App::App()
{
	zprint("%s %s", __DATE__, __TIME__);
    RECT r;
	WindowPosition::Get<MainWindow>(r);
	HWND h = WindowTemplate(&mainWindow, L"События", r.left, r.top, r.right, r.bottom, IDI_LETTER);
	ShowWindow(h, SW_SHOWNORMAL);
}

void App::Destroy()
{
}