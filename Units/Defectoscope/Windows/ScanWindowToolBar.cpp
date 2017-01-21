#include "stdafx.h"
#include "ScanWindowToolBar.h"
#include "Resource.h"
#include "window_tool/InitToolBar.hpp"
#include "debug_tools/DebugMess.h"
#include "ScanWindow.h"

namespace
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
	KEY(IDB_DownArrow , L"- Датчик")
	KEY(IDB_UpArrow   , L"+ Датчик")
	KEY(IDB_LeftArrow , L"- Смещение")
	KEY(IDB_RightArrow, L"+ Смещение")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_DownArrow) 
		, BUTTON_KEY(IDB_UpArrow)
		, BUTTON_KEY(IDB_LeftArrow) 
		, BUTTON_KEY(IDB_RightArrow)
		, SeparatorToolbar<2>
		>::Result tool_button_list;
#undef BUTTON_KEY
	void Key<IDB_DownArrow>::Click(HWND h)
	{
		ScanWindow *w = (ScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		w->SensMinus();
	}
//------------------------------------------------------------------------------
	void Key<IDB_UpArrow>::Click(HWND h)
	{
		ScanWindow *w = (ScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		w->SensPlus();
	}
//------------------------------------------------------------------------------
	void Key<IDB_LeftArrow>::Click(HWND h)
	{
		ScanWindow *w = (ScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		w->OffsMinus();
	}
//------------------------------------------------------------------------------
	void Key<IDB_RightArrow>::Click(HWND h)
	{
		ScanWindow *w = (ScanWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		w->OffsPlus();
	}
}

HWND ScanWindowToolBar::Init(HWND h)
{
	return hWnd = InitToolbar<tool_button_list, 16>()(h);
}
void ScanWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
