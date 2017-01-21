#include "stdafx.h"
#include "App.h"
#include "App/Config.h"
#include "debug_tools/DebugMess.h"
#include "DiffWindows/MainWindow.h"
#include "Ini/WindowsPosition.h"
#include "window_tool/EmptyWindow.h"
#include "1730/Device1730.h"
#include "App/AppBase.h"
#include "Automat/Automat.h"
#include "ItemsData/USPCData.h"
#include "ItemsData/ConstData.h"
#include "Compute/Compute.h"
#include "USPC/ut_files.h"
#include "Automat/Automat.h"
#include "window_tool/HookKey.h"
#include "USPC/LogUSPC.h"
#include "window_tool\ZipAll.h"
#include "Stored/StoredBase.h"
#include "Base/ExpressBase.hpp"
#include "Stored/Stored.h"
#include "USPC\USPC.h"


HANDLE App::ProgrammExitEvent;
HANDLE App::ProgrammContinueEvent;
HANDLE App::ProgrammStopEvent;
HANDLE App::ProgrammRunEvent;
bool App::measurementOfRunning = false;
int __lengthCaretka = 0;
const int &App::lengthCaretka = __lengthCaretka;
//---------------------------------------------------------------
App::App()	
	: mainWindow(Singleton<MainWindow>::Instance())
{}

namespace
{
	template<class O, class P>struct __caretka__
	{
		void operator()(O *o, P *p)
		{
			if(o->value > *p) *p = o->value;
		}
	};
}

void App::InitCaretka()
{
	__lengthCaretka = 0;
	TL::foreach<OffsetsTable::items_list, __caretka__>()(&Singleton<OffsetsTable>::Instance().items, &__lengthCaretka);
}

void App::Init()
{
	AppBase().Init();
	ConstData::Init();
	App::ProgrammExitEvent		= CreateEvent(NULL, TRUE, FALSE, NULL);
	App::ProgrammContinueEvent	= CreateEvent(NULL, TRUE, FALSE, NULL);
	App::ProgrammStopEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
	App::ProgrammRunEvent   = CreateEvent(NULL, FALSE, FALSE, NULL);
	InitCaretka();
	LogUSPC::Clear();
	RECT r;
	WindowPosition::Get<MainWindow>(r);
	wchar_t b[256];
	wchar_t bb[512];
	ExistCurrentUSPCFile(b);
	wsprintf(bb, L"%s %s", App::TitleApp(), b);
	HWND h = WindowTemplate(&mainWindow, bb, r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
	StartKeyHook(h);
	if(!device1730.Init(Singleton<Descriptor1730Table>::Instance().items.get<Descriptor1730>().value))
	{
		MessageBox(h, L"Не могу инициировать плату 1730", L"Ошибка !!!", MB_ICONERROR);
#ifndef DEBUG_ITEMS
		return;
#endif
	}
	USPC::ConfigFromFile();
	Stored::CleanStoredBase();
	automat.Init();
	Zip::ZipAll();
}

void App::Destroy()
{
	device1730.Write(0);
	SetEvent(ProgrammExitEvent);
	Sleep(1000);
}

void App::MainWindowTopLabel(wchar_t *txt)
{
	mainWindow.topLabelViewer.SetMessage(txt);
}

void App::MainWindowBottomLabel(int n, wchar_t *text)
{
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, n, (LONG)text);
}

void App::MainWindowUpdate()
{
	RepaintWindow(mainWindow.hWnd);
}
HWND App::MainWindowHWND()
{
	return app.mainWindow.hWnd;
}

Device1730 device1730;

App app;

Automat automat;





