#include "stdafx.h"
#include "ThicknessThresholdWindow.h"
#include "debug_tools/DebugMess.h"
#include "debug_tools/DebugMess.h"
#include "ItemsData/ConstData.h"
//#include "window_tool/MenuApi.h"
#include "ViewersMenu.hpp"
#include "CrossWindow.h"
#include "Common.h"
#include "window_tool/InitToolBar.hpp"
#include "AxesDlg.hpp"
#include "DiffApp/App.h"

using namespace Gdiplus;
//-----------------------------------------------------------------------------------------
ThicknessThresholdWindow::ThicknessThresholdWindow()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, alignOneZone 	(*this)
	, alignAllZones	(*this)
	, alignThreshold(*this)
	, offset01(*this)
	, offset50(*this)
	, fastOffset(false)
	, minAxesY(Singleton<AxesTable>::Instance().items.get<AxesYMin<Thickness> >().value)
	, maxAxesY(Singleton<AxesTable>::Instance().items.get<AxesYMax<Thickness> >().value)
{
	chart.minAxesX = 0;
	chart.maxAxesX = App::count_zones;
	chart.rect.top = 17;
	mouseMove = true;
	label.fontHeight = 12;
	cursor.SetMouseMoveHandler(this, &ThicknessThresholdWindow::Draw);	

	AboveThresh   &at = chart.items.get<AboveThresh  >();
	LowerThresh   &lt = chart.items.get<LowerThresh  >();
	NominalThresh &nt = chart.items.get<NominalThresh>();

	at.SetData(aboveBorder  , App::count_zones, 0, 255);
	lt.SetData(lowerBorder  , App::count_zones, 0, 255);
	nt.SetData(nominalBorder, App::count_zones, 0, 255);

	at.color = 0xff0000ff;
	lt.color = 0xffff0000;
	nt.color = 0xff00ff00;
}
//--------------------------------------------------------------------------
bool ThicknessThresholdWindow::Draw(TMouseMove &l, VGraphics &g)
{
	int x, y;
	chart.CoordCell(l.x, l.y, x, y);
	double nominal = nominalBorder[x];
	wsprintf(label.buffer, L"<ff>зона <ff00>%d  <ff>\"Номинальная толщина\" <ff0000>%s  <ff>\"Верхняя граница\" <ffff00>%s  <ff>\"Нижняя граница\" <ffff00>%s     "
	  , 1 + x
	  , Wchar_from<double, 5>(nominal)()
	  , Wchar_from<double, 5>(aboveBorder[x])()
	  , Wchar_from<double, 5>(lowerBorder[x])()
	  );
	label.Draw(g());

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------
void ThicknessThresholdWindow::operator()(TPaint &l)
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
void ThicknessThresholdWindow::operator()(TMouseMove &l)
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
void ThicknessThresholdWindow::operator()(TLButtonDbClk &l)
{
	 mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
//--------------------------------------------------------------------------------
void ThicknessThresholdWindow::operator()(TMouseWell &l)
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
	double incr = 0 == (l.flags.rButton || fastOffset) ? 0.1 : 0.5;
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
void ThicknessThresholdWindow::operator()(TLButtonDown &)
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
			ThicknessThresholdWindow *o = (ThicknessThresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
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
			ThicknessThresholdWindow *o = (ThicknessThresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
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
			ThicknessThresholdWindow *o = (ThicknessThresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
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
			ThicknessThresholdWindow *o = (ThicknessThresholdWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
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

TOP_MENU(Thickness)
unsigned ThicknessThresholdWindow::operator()(TCreate &l)
{
	Menu<TopMenuThickness::MainMenu>().Init(l.hwnd);
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;
	storedMouseMove.y = 0;
	
	hToolBar = __tool_bar__()(l.hwnd);

	aboveBorderCheckBox.Init(hToolBar, L"Верхняя граница");
	lowerBorderCheckBox.Init(hToolBar, L"Нижняя граница");
	nominalBorderCheckBox.Init(hToolBar, L"Номинальная толщина");

	offset01.Init(hToolBar, L"Смещение 0.1", true, true);
	offset50.Init(hToolBar, L"Смещение 0.5");

	alignOneZone.Init(hToolBar, L"Сместить зону", true, true);
	alignAllZones.Init(hToolBar, L"Выровнять по зоне");
	alignThreshold.Init(hToolBar, L"Сместить все зоны");

	ptrAlign = &ThicknessThresholdWindow::AlignOneZone;

	memmove(aboveBorder
		, Singleton<ThresholdsTable>::Instance().items.get<BorderAbove  <Thickness> >().value
		, sizeof(aboveBorder)
		);
	memmove(lowerBorder
		, Singleton<ThresholdsTable>::Instance().items.get<BorderLower  <Thickness> >().value
		, sizeof(lowerBorder)
		);

	memmove(nominalBorder
		, Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<Thickness> >().value
		, sizeof(nominalBorder)
		);

	for(int i = 0; i < dimention_of(nominalBorder); ++i)
	{
		aboveBorder[i] += nominalBorder[i];
		lowerBorder[i] =  nominalBorder[i] - lowerBorder[i];
	}

	return 0;
}
//------------------------------------------------------------------------------------------
void ThicknessThresholdWindow::operator()(TSize &l)
{
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
	chart.minAxesY = minAxesY;
	chart.maxAxesY = maxAxesY;
	Draw(l);
//----toolbar
	int tool_bar_width = 320;
	aboveBorderCheckBox  .Size(tool_bar_width, 10, 160, 20);
	lowerBorderCheckBox  .Size(tool_bar_width, 50, 160, 20);
	nominalBorderCheckBox.Size(tool_bar_width, 30, 160, 20);
	tool_bar_width += 180;
	offset01.Size(tool_bar_width, 10, 120, 20);
	offset50.Size(tool_bar_width, 30, 120, 20);

	tool_bar_width += 130;
	alignOneZone  .Size(tool_bar_width, 10, 170, 20);
	alignAllZones .Size(tool_bar_width, 50, 170, 20);
	alignThreshold.Size(tool_bar_width, 30, 170, 20);
}
//------------------------------------------------------------------------------------------
void ThicknessThresholdWindow::Draw(TSize &l)
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
void ThicknessThresholdWindow::operator()(TKeyDown &l)
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
void ThicknessThresholdWindow::operator()(TCommand &l)
{
	EventDo(l);
}
//----------------------------------------------------------------------------------------------
void ThicknessThresholdWindow::AlignOneZone  (int x, double offs)
{
	if(aboveBorderCheckBox  .value)aboveBorder  [x] += offs;
	if(lowerBorderCheckBox  .value)lowerBorder  [x] += offs;
	if(nominalBorderCheckBox.value)nominalBorder[x] += offs;
	SetFocus(hWnd);
}
//---------------------------------------------------------------------------
void ThicknessThresholdWindow::AlignAllZones (int x, double offs)
{
	if(aboveBorderCheckBox.value)
	{
		double t = aboveBorder[x];
		for(int i = 0; i < App::count_zones; ++i) aboveBorder[i] = t;
	}
	if(lowerBorderCheckBox.value)
	{
		double t = lowerBorder[x];
		for(int i = 0; i < App::count_zones; ++i) lowerBorder[i] = t;
	}
	if(nominalBorderCheckBox.value)
	{
		double t = nominalBorder[x];
		for(int i = 0; i < App::count_zones; ++i) nominalBorder[i] = t;
	}
	SetFocus(hWnd);
}
//------------------------------------------------------------------------------------
void ThicknessThresholdWindow::AlignThresh(int x, double offs)
{
	if(aboveBorderCheckBox  .value)for(int i = 0; i < App::count_zones; ++i) aboveBorder  [i] += offs;
	if(lowerBorderCheckBox  .value)for(int i = 0; i < App::count_zones; ++i) lowerBorder  [i] += offs;
	if(nominalBorderCheckBox.value)for(int i = 0; i < App::count_zones; ++i) nominalBorder[i] += offs;
	SetFocus(hWnd);
}
//-----------------------------------------------------------------------------------------------------
namespace
{
	template<void(ThicknessThresholdWindow::*)(int, double)>struct __x__{};

	template<class O, class P>struct __ret__;

	template<class P>struct __ret__<__x__<&ThicknessThresholdWindow::AlignOneZone>, P>
	{
		bool operator()(P *p)
		{
			if(p->ptrAlign == &ThicknessThresholdWindow::AlignOneZone)
			{
				Button_SetCheck(p->alignOneZone.hWnd, BST_CHECKED);
				return false;
			}
			return true;
		}
	};
	template<class P>struct __ret__<__x__<&ThicknessThresholdWindow::AlignThresh>, P>
	{
		bool operator()(P *p)
		{
			if(p->ptrAlign == &ThicknessThresholdWindow::AlignThresh)
			{
				Button_SetCheck(p->alignThreshold.hWnd, BST_CHECKED);
				return false;
			}
			return true;
		}
	};

	typedef TL::MkTlst<__x__<&ThicknessThresholdWindow::AlignOneZone>, __x__<&ThicknessThresholdWindow::AlignThresh> >::Result lst;
}
void ThicknessThresholdWindow::AlignThreshold<&ThicknessThresholdWindow::AlignAllZones>::Command(TCommand &m)
{
	TL::find<lst, __ret__>()(&owner);
	Button_SetCheck(m.hControl, BST_UNCHECKED);
	int x, y;
	owner.chart.CoordCell( owner.storedMouseMove.x, owner.storedMouseMove.y, x, y);
	(owner.ThicknessThresholdWindow::AlignAllZones)(x, 0);
	RepaintWindow(m.hwnd);
}
//-------------------------------------------------------------------------------------
void ThicknessThresholdWindow::operator()(TClose &l)
{
	ThresholdsTable &table = Singleton<ThresholdsTable>::Instance();
	bool changedNominal = false;
	double *data = table.items.get<BorderNominal<Thickness> >().value;
	for(int i = 0; i < dimention_of(nominalBorder); ++i)
	{
		if(data[i] != nominalBorder[i])
		{
			changedNominal = true;
			break;
		}
	}

	for(int i = 0; i < dimention_of(nominalBorder); ++i)
	{
		aboveBorder[i] -= nominalBorder[i];
		lowerBorder[i] = nominalBorder[i] - lowerBorder[i];
	}

	bool changedAbove = false;
	data = table.items.get<BorderAbove  <Thickness> >().value;
	for(int i = 0; i < dimention_of(aboveBorder); ++i)
	{
		if(data[i] != aboveBorder[i])
		{
			changedAbove = true;
			break;
		}
	}
	bool changedLower = false;
	data = table.items.get<BorderLower  <Thickness> >().value;
	for(int i = 0; i < dimention_of(lowerBorder); ++i)
	{
		if(data[i] != lowerBorder[i])
		{
			changedLower = true;
			break;
		}
	}
	
	if(changedAbove || changedLower || changedNominal)
	{
		int ret = MessageBox(l.hwnd, L"Данные были изменены!\nСохранить?", L"Cообщение", MB_ICONQUESTION | MB_YESNOCANCEL);
		if(IDYES == ret)
		{
			if(TestPassword<ParametersBase, ThresholdsTable>()(l.hwnd))
			{
				if(changedNominal)
				{
					memmove(
						table.items.get<BorderNominal<Thickness> >().value
						, nominalBorder
						, sizeof(nominalBorder)
						);
				}
				if(changedAbove)
				{					
					memmove(
						table.items.get<BorderAbove  <Thickness> >().value
						, aboveBorder
						, sizeof(aboveBorder)
						);
				}
				if(changedLower)
				{
					memmove(
						table.items.get<BorderLower  <Thickness> >().value
						, lowerBorder
						, sizeof(lowerBorder)
						);
				}
				CBase base(ParametersBase().name());
				if(base.IsOpen())
				{
					int id = CurrentId<ID<ThresholdsTable> >();

					if(1 == CountId<ID<ThresholdsTable> >(base, id))
					{
						Update<ThresholdsTable> update(base);
						if(changedNominal)  update.set<BorderNominal<Thickness> >(nominalBorder);
						if(changedAbove)  update.set<BorderAbove  <Thickness> >(aboveBorder);
						if(changedLower)  update.set<BorderLower  <Thickness> >(lowerBorder);
						update.Where().ID(id).Execute();
					}
					else
					{
						Insert_Into<ThresholdsTable>(table, base).Execute();
						int id = Select<ThresholdsTable>(base).eq_all<ThresholdsTable::items_list>(&table.items).Execute();
						UpdateId<ID<ThresholdsTable> >(base, id);
					}
				}
			}
		}
		if(IDCANCEL == ret) return;
	}
	DestroyWindow(l.hwnd);
}
//--------------------------------------------------------------------------------------
wchar_t *ThicknessThresholdWindow::Title()
{
	return L"Пороги толщины";
}
//----------------------------------------------------------------------------------------
void ThicknessThresholdWindow::operator()(TDestroy &m)
{
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
	delete backScreen;
}
//----------------------------------------------------------------------------------------