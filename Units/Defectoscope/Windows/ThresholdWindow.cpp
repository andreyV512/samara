#include "stdafx.h"
#include "ThresholdWindow.h"
#include "debug_tools/DebugMess.h"
#include "debug_tools/DebugMess.h"
#include "ItemsData/ConstData.h"
#include "window_tool/MenuApi.h"
#include "ViewersMenu.hpp"
#include "CrossWindow.h"
#include "Common.h"
#include "window_tool/InitToolBar.hpp"

using namespace Gdiplus;
//-----------------------------------------------------------------------------------------
TresholdWindow::TresholdWindow()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, alignOneZone 	(*this)
	, alignAllZones	(*this)
	, alignThreshold(*this)
	, offset01(*this)
	, offset50(*this)
{
	chart.minAxesY = 0;
	chart.maxAxesY = 255;
	chart.minAxesX = 0;
	chart.maxAxesX = App::count_zones;
	chart.rect.top = 17;
	mouseMove = true;
	label.fontHeight = 12;
	cursor.SetMouseMoveHandler(this, &TresholdWindow::Draw);	
	BrackThresh &bt = chart.items.get<BrackThresh>();
	bt.SetData(brak, App::count_zones, 0, 255);
	bt.color = 0xffff0000;
	Klass2Thresh &kt = chart.items.get<Klass2Thresh>();
	kt.SetData(klass2, App::count_zones, 0, 255);
	kt.color = 0xffffff00;
	fastOffset = false;
}
//--------------------------------------------------------------------------
bool TresholdWindow::Draw(TMouseMove &l, VGraphics &g)
{
	int x, y;
	chart.CoordCell(l.x, l.y, x, y);
	
	wsprintf(label.buffer, L"<ff>зона <ff00>%d  <ff>\"Брак\" <ff0000>%s  <ff>\"Класс2\" <ffff00>%s       "
	  , 1 + x
	  , Wchar_from<double, 5>(brak[x])()
	  , Wchar_from<double, 5>(klass2[x])()
	  );
	label.Draw(g());

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
void TresholdWindow::operator()(TPaint &l)
{
	if(NULL == backScreen) return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor.VerticalCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}
//------------------------------------------------------------------------
void TresholdWindow::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}
//------------------------------------------------------------------------------
void TresholdWindow::operator()(TLButtonDbClk &l)
{
	 mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
//--------------------------------------------------------------------------------
void TresholdWindow::operator()(TMouseWell &l)
{
	if(0 == storedMouseMove.x)
	{
		storedMouseMove.x = (WORD)(chart.rect.right - chart.rect.left) / 2;
	}
	if(0 == storedMouseMove.y)
	{
		storedMouseMove.y = (WORD)(chart.rect.top + 3);
	}
	mouseMove = false;
	int d = l.delta / 120;
	bool m = 0 == (l.flags.lButton || l.flags.rButton);
	double incr = 0 == (l.flags.rButton || fastOffset)  ? 1 : 5;
	OffsetToPixel(
		chart
		, storedMouseMove.x
		, storedMouseMove.y
		, d
		, m 
		);
	if(!m)
	{
		int x, y;
		chart.CoordCell( storedMouseMove.x, storedMouseMove.y, x, y);
		(this->*ptrAlign)(x, d > 0 ? -incr : incr);
		RECT r;
		GetClientRect(l.hwnd, &r);
		TSize size = {l.hwnd, WM_SIZE, 0, (WORD)r.right, (WORD)r.bottom};
		Draw(size);
	}
	cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));	
}
//--------------------------------------------------------------------------------------
void TresholdWindow::operator()(TLButtonDown &)
{
		mouseMove = false;
}
//------------------------------------------------------------------
namespace
{
	struct LeftBtn
	{
		static const int IDB_ = IDB_LeftArrow64;
		static void Proc(HWND h)
		{
			TresholdWindow *o = (TresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			TMouseWell mouseWheel = {h, WM_MOUSEWHEEL, 0, 0, 0, 0};
			mouseWheel.delta = 120;
			(*o)(mouseWheel);
		}
	};
	struct RightBtn
	{
		static const int IDB_ = IDB_RightArrow64;
		static void Proc(HWND h)
		{
			TresholdWindow *o = (TresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			TMouseWell mouseWheel = {h, WM_MOUSEWHEEL, 0, 0, 0, 0};
			mouseWheel.delta = -120;
			(*o)(mouseWheel);
		}
	};
	struct UpBtn
	{
		static const int IDB_ = IDB_UpArrow64;
		static void Proc(HWND h)
		{
			TresholdWindow *o = (TresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			TMouseWell mouseWheel = {h, WM_MOUSEWHEEL, 0, 0, 0, 0};
			mouseWheel.delta = -120;
			mouseWheel.flags.lButton = 1;
			mouseWheel.flags.rButton = o->fastOffset;
			(*o)(mouseWheel);
		}
	};
	struct DownBtn
	{
		static const int IDB_ = IDB_DownArrow64;
		static void Proc(HWND h)
		{
			TresholdWindow *o = (TresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			TMouseWell mouseWheel = {h, WM_MOUSEWHEEL, 0, 0, 0, 0};
			mouseWheel.delta = 120;
			mouseWheel.flags.lButton = 1;
			mouseWheel.flags.rButton = o->fastOffset;
			(*o)(mouseWheel);
		}
	};

	typedef MkToolBarLst<TL::MkTlst<
		SeparatorToolbar<0>
		, LeftBtn
		, RightBtn
		, UpBtn
		, DownBtn
		, SeparatorToolbar<1>
	>::Result>::Result __tool_bar_tool_list__;
	typedef InitToolbar<__tool_bar_tool_list__, 64> __tool_bar__;
}
unsigned TresholdWindow::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;
	storedMouseMove.y = 0;
	
	hToolBar = __tool_bar__()(l.hwnd);

	border2ClassCheckBox.Init(hToolBar, L"Порог \"Класс 2\"");
	borderDefectCheckBox.Init(hToolBar, L"Порог \"Брак\"");

	offset01.Init(hToolBar, L"Смещение 1", true, true);
	offset50.Init(hToolBar, L"Смещение 5");

	alignOneZone.Init(hToolBar, L"Сместить зону", true, true);
	alignAllZones.Init(hToolBar, L"Выровнять по зоне");
	alignThreshold.Init(hToolBar, L"Сместить все зоны");

	ptrAlign = &TresholdWindow::AlignOneZone;

	return 0;
}
//------------------------------------------------------------------------------------------
void TresholdWindow::operator()(TSize &l)
{
	storedMouseMove.hwnd = l.hwnd;
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	
	
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
	MoveWindow(hToolBar, 0, 0, 0, 0, true);
	Draw(l);
//----toolbar
	int tool_bar_width = 320;
	border2ClassCheckBox.Size(tool_bar_width, 30, 120, 20);
	borderDefectCheckBox.Size(tool_bar_width, 10, 120, 20);

	tool_bar_width += 140;
	offset01.Size(tool_bar_width, 10, 120, 20);
	offset50.Size(tool_bar_width, 30, 120, 20);

	tool_bar_width += 140;
	alignOneZone  .Size(tool_bar_width, 10, 170, 20);
	alignAllZones .Size(tool_bar_width, 50, 170, 20);
	alignThreshold.Size(tool_bar_width, 30, 170, 20);
}
//------------------------------------------------------------------------------------------
void TresholdWindow::Draw(TSize &l)
{
	RECT r = {};
	GetClientRect(hToolBar, &r);
	if(0 == r.bottom)  return;
    Graphics g(backScreen);
	SolidBrush solidBrush(Color(0xffaaaaaa));
	g.FillRectangle(&solidBrush, 0, r.bottom, l.Width, 20);  
	label.top = r.bottom;
	chart.rect.top = r.bottom + 20;
	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;
	chart.Draw(g);
}
//-------------------------------------------------------------------------------------------
void TresholdWindow::operator()(TKeyDown &l)
{
	TMouseWell mouseWheel = {l.hwnd, WM_MOUSEWHEEL, 0, 0, 0, 0};
	switch(l.VirtKey)
	{
	case VK_LEFT:
		mouseWheel.delta = 120;
		break;
	case VK_RIGHT:
		mouseWheel.delta = -120;
		break;
	case VK_UP:
		mouseWheel.delta = -120;
		mouseWheel.flags.lButton = 1;
		mouseWheel.flags.rButton = fastOffset || 0 > GetKeyState(VK_SHIFT);
		break;
	case VK_DOWN:
		mouseWheel.delta = 120;
		mouseWheel.flags.lButton = 1;
		mouseWheel.flags.rButton = fastOffset || 0 > GetKeyState(VK_SHIFT);
		break;
	default: return;
	}
	
	(*this)(mouseWheel);
}
//---------------------------------------------------------------------------
void TresholdWindow::operator()(TCommand &l)
{
	EventDo(l);
}
//----------------------------------------------------------------------------------------------
void TresholdWindow::AlignOneZone  (int x, double offs)
{
	if(borderDefectCheckBox.value)brak[x] += offs;
	if(border2ClassCheckBox.value)klass2[x] += offs;
	SetFocus(hWnd);
}
//---------------------------------------------------------------------------
void TresholdWindow::AlignAllZones (int x, double offs)
{
	if(borderDefectCheckBox.value)
	{
		double t = brak[x];
		for(int i = 0; i < App::count_zones; ++i) brak[i] = t;
	}
	if(border2ClassCheckBox.value)
	{
		double t = klass2[x];
		for(int i = 0; i < App::count_zones; ++i) klass2[i] = t;
	}
	SetFocus(hWnd);
}
//------------------------------------------------------------------------------------
void TresholdWindow::AlignThresh(int x, double offs)
{
	if(borderDefectCheckBox.value)for(int i = 0; i < App::count_zones; ++i) brak[i] += offs;
	if(border2ClassCheckBox.value)for(int i = 0; i < App::count_zones; ++i) klass2[i] += offs;
	SetFocus(hWnd);
}
//-----------------------------------------------------------------------------------------------------
namespace
{
	template<void(TresholdWindow::*)(int, double)>struct __x__{};

	template<class O, class P>struct __ret__;

	template<class P>struct __ret__<__x__<&TresholdWindow::AlignOneZone>, P>
	{
		bool operator()(P *p)
		{
			if(p->ptrAlign == &TresholdWindow::AlignOneZone)
			{
				Button_SetCheck(p->alignOneZone.hWnd, BST_CHECKED);
				return false;
			}
			return true;
		}
	};
	template<class P>struct __ret__<__x__<&TresholdWindow::AlignThresh>, P>
	{
		bool operator()(P *p)
		{
			if(p->ptrAlign == &TresholdWindow::AlignThresh)
			{
				Button_SetCheck(p->alignThreshold.hWnd, BST_CHECKED);
				return false;
			}
			return true;
		}
	};

	typedef TL::MkTlst<__x__<&TresholdWindow::AlignOneZone>, __x__<&TresholdWindow::AlignThresh> >::Result lst;
}
void TresholdWindow::AlignThreshold<&TresholdWindow::AlignAllZones>::Command(TCommand &m)
{
	TL::find<lst, __ret__>()(&owner);
	Button_SetCheck(m.hControl, BST_UNCHECKED);
	int x, y;
	owner.chart.CoordCell( owner.storedMouseMove.x, owner.storedMouseMove.y, x, y);
	(owner.TresholdWindow::AlignAllZones)(x, 0);
	RepaintWindow(m.hwnd);
}
//-------------------------------------------------------------------------------------
void TresholdWindow::operator()(TDestroy &m)
{
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
	delete backScreen;	
}
//-----------------------------------------------------------------------------------