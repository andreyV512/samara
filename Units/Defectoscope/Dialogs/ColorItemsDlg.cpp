#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "debug_tools/DebugMess.h"
#include "window_tool\Dlgfiles.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "ItemsData/ConstData.h"

namespace{
void __color_wm_paint(HWND hwnd, int color)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(hwnd, &p);
	{		
		Gdiplus::Graphics g(hdc);
		RECT r;
		GetClientRect(hwnd, &r);
		g.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(color)), 0, 0, r.right, r.bottom);
	}
	EndPaint(hwnd, &p);
}

int __color_l_button_down(HWND hwnd)
{
	ColorDialog c(hwnd);
	char d[4] = {-1};
	if(c())
	{
		*(int *)d = c.color.rgbResult | 0xff000000;
		char t = d[0];
		d[0] = d[2];
		d[2] = t;
		InvalidateRect(hwnd, NULL, true);
	}
	return *(int *)d;
}

template<class T>struct __color__
{	
	static LRESULT CALLBACK Proc_(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_PAINT:
			{
				T *t = (T *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				if(t)
				{
					__color_wm_paint(hwnd, t->value);
				}
			}
			return 0;
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
			{
				T *t = (T *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				if(t)
				{
			    t->value = __color_l_button_down(hwnd);
				}
			}
			return 0;
		}
		return CallWindowProc(OldWndProc, hwnd, msg, wParam, lParam);
	}
	static WNDPROC OldWndProc;
};

 template<class T>WNDPROC __color__<T>::OldWndProc;

template<class T>struct ColorButtonItem
{
	HWND Init(HWND h, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindow(L"button", L""
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			,10, dy, 100, 30, h, (HMENU)0, (HINSTANCE)::GetModuleHandle(NULL), 0
			);
		__color__<T>::OldWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (long ) __color__<T>::Proc_);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (long )&t);
		CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, 100 + 20, dy + 7, dlg_width, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += 35;

		return hWnd;
	}
};

#define DLG_SUB(n)template<>struct DlgSubItems<n, int>: ColorButtonItem<n>{};\
template<>struct __data_from_widget__<DlgItem<n>, int>\
{\
    int operator()(DlgItem<n> &o)\
	{\
		return o.value.value;\
	}\
};
#define	JOIN2(a, b) a##,##b
DLG_SUB(Clr<Undefined>			)
DLG_SUB(Clr<Nominal  >			)
DLG_SUB(Clr<DeathZone>			)
DLG_SUB(Clr<BorderKlass2<Long>      >)
DLG_SUB(Clr<BorderDefect<Long>      >)
DLG_SUB(Clr<BorderKlass2<Cross>     >)
DLG_SUB(Clr<BorderDefect<Cross>     >)
DLG_SUB(Clr<BorderAbove<Thickness>  >)
DLG_SUB(Clr<BorderLower<Thickness>  >)
DLG_SUB(Clr<BrakStrobe2<Thickness>>)
DLG_SUB(Clr<Cancel<Projectionist>>)
//------------------------------------------------------------------------------------------------
PARAM_TITLE(Clr<Undefined>			 , L"Результат не определён")
PARAM_TITLE(Clr<Nominal  >			 , L"\"1 класс\"")
PARAM_TITLE(Clr<DeathZone>			 , L"Мёртвая зона")
PARAM_TITLE(Clr<BorderLower<Thickness> > , L"Толщина меньше нормы")
PARAM_TITLE(Clr<BorderAbove<Thickness> > , L"Толщина больше нормы")
PARAM_TITLE(Clr<BorderKlass2<Long>      > , L"\"2 класс\" продольный дефект")
PARAM_TITLE(Clr<BorderDefect<Long>      > , L"продольный дефект брак")
PARAM_TITLE(Clr<BorderKlass2<Cross>     > , L"\"2 класс\" поперечный дефект")
PARAM_TITLE(Clr<BorderDefect<Cross>     > , L"поперечный дефект брак")
PARAM_TITLE(JOIN2(Clr<BorderLower<Thickness>, BorderAbove<Thickness>>), L"Толщина меньше и больше нормы")
PARAM_TITLE(Clr<BrakStrobe2<Thickness>>, L"Расслоение")
PARAM_TITLE(Clr<Cancel<Projectionist>>, L"отмена")
DO_NOT_CHECK(Clr<Undefined>			)
DO_NOT_CHECK(Clr<Nominal  >			)
DO_NOT_CHECK(Clr<DeathZone>			)
DO_NOT_CHECK(Clr<BorderAbove<Thickness>> )
DO_NOT_CHECK(Clr<BorderLower<Thickness>>)
DO_NOT_CHECK(Clr<BorderKlass2<Long>     >)
DO_NOT_CHECK(Clr<BorderDefect<Long>     >)
DO_NOT_CHECK(Clr<BorderKlass2<Cross>    >)
DO_NOT_CHECK(Clr<BorderDefect<Cross>    >)
DO_NOT_CHECK(JOIN2(Clr<BorderLower<Thickness>, BorderAbove<Thickness>>))
DO_NOT_CHECK(Clr<BrakStrobe2<Thickness>>)
DO_NOT_CHECK(Clr<Cancel<Projectionist>>)

#undef	JOIN2

struct DefaultBtn
{
	static const int width = 160;
	static const int height = 30;
	static const int ID = IDRETRY;
	wchar_t *Title(){return L"Встроенные настройки";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		if(TestPassword<Owner::Base, Owner::Table>()(h))
		{
			CBase base(Owner::Base().name());
			if(base.IsOpen())
			{
				ColorTable color;
				UpdateWhere<ColorTable>(color, base).ID(1).Execute();
				CopyFactory::Do(Singleton<ColorTable>::Instance().items, color.items);
			}
			EndDialog(h, FALSE);
		}
	}
};
}
void ColorItemsDlg::Do(HWND h)
{
	ColorTable color;
	CopyFactory::Do(color.items, Singleton<ColorTable>::Instance().items);
	if(TemplDialog<ParametersBase, ColorTable
		 , TL::MkTlst<OkBtn, CancelBtn, DefaultBtn>::Result>(color).Do(h, L"Цвета"))
	{
		CopyFactory::Do(Singleton<ColorTable>::Instance().items, color.items);
		ConstData::Init();
	}
}

