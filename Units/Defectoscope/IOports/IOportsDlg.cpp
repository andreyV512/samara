#pragma once
#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "IOportsWindow.h"
#include "window_tool\EmptyWindow.h"
#include "App/AppBase.h"
#include "DiffWindows/mainwindow.h"
#include "window_tool\Pass.h"
#include <CommCtrl.h>
#include <WindowsX.h>
#include "ini/WindowsPosition.h"

void IOportsDlg::Do(HWND h)
{
	HWND hh = FindWindow(L"IOportsWindow", 0);
	if(NULL != hh)
	{
		SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(hh);
	}
	else
	{
		RECT r;
		WindowPosition::Get<IOportsWindow>(r);
		wchar_t *title = App::measurementOfRunning
			? L"Просмотр дискретных входов-выходов"
			: L"Управление дискретными выходами"
		;
		h = WindowTemplate(
			 &IOportsWindow::Instance()
			, title
			, r.left, r.top
			, IOportsWindow::width, IOportsWindow::height
			);
		ShowWindow(h, SW_SHOWNORMAL);
		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		DWORD dwStyle = GetWindowLongPtr(h, GWL_STYLE);
		dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
		SetWindowLongPtr(h, GWL_STYLE, dwStyle);
	}
}

