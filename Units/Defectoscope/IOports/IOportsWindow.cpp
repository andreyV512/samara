#include "stdafx.h"
#include "IOportsWindow.h"
#include "window_tool/MenuApi.h"
#include "IOportsWindowMenu.hpp"
#include "window_tool\EmptyWindow.h"
#include "debug_tools/DebugMess.h"

using namespace Gdiplus;
//------------------------------------------------------------------------
IOportsWindow::IOportsWindow() 
	: backScreen(NULL)
	, viewer(hWnd, backScreen)
{}
//-------------------------------------------------------------------------
void IOportsWindow::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || l.Height < 100) return;	
	
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else if(l.Width > 0 && l.Height > 0)
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	else
	{
		return;
	}
	
	Graphics g(backScreen);
	viewer.Size(g, l.Width, l.Height);
}
//------------------------------------------------------------------------
void IOportsWindow::operator()(TCommand &m)
{
	EventDo(m);
}
//------------------------------------------------------------------------
void IOportsWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = width;
		m.pMinMaxInfo->ptMinTrackSize.y = height;
		m.pMinMaxInfo->ptMaxTrackSize.x = width;
		m.pMinMaxInfo->ptMaxTrackSize.y = height;		
	}		
}
//------------------------------------------------------------------------
unsigned IOportsWindow::operator()(TCreate &m)
{
	Menu<IOportsWindowMenu::MainMenu>().Init(m.hwnd);
	viewer.Start();
	return 0;
}
//------------------------------------------------------------------------
void IOportsWindow::operator()(TDestroy &m)
{
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
	viewer.Stop();
	Sleep(500);
	delete backScreen;
    backScreen = NULL;	
}
//------------------------------------------------------------------------
void IOportsWindow::operator()(TLButtonDown &l)
{
		viewer.MouseLDown(l);
}
//------------------------------------------------------------------------
void IOportsWindow::operator()(TPaint &l)
{
	if(NULL == backScreen)return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);		
	}
	EndPaint(l.hwnd, &p);
}

IOportsWindow &IOportsWindow::Instance(){static IOportsWindow x; return x;};
