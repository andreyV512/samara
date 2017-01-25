#include "stdafx.h"
#include "MainWindowToolBar.h"
#include "App/Config.h"
#include "debug_tools/DebugMess.h"
#include "Resource.h"
#include "window_tool/InitToolBar.hpp"
#include "window_tool/message.h"
#include "window_tool/Emptywindow.h"
#include "MainWindow.h"
#include "Windows/Common.h"
#include "Windows/PacketWindow.h"
#include <gdiplus.h>
#include "Automat\AppKeyHandler.h"
#include "Stored\Stored.h"
#include "window_tool\ToolBarButton.h"
#include "App/Config.h"
#include "Dialogs\NumberTubeDlg.h"
using namespace Gdiplus;
namespace 
{
	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
	KEY(IDB_CycleBtn   , L"F4 Цикл")
	KEY(IDB_Reset      , L"F9 Стоп")
	KEY(IDB_QueryBtn   , L"Тест")
	KEY(IDB_arrow_down , L"F5 Вниз")
	KEY(IDB_arrow_up   , L"F6 БАЗА")
	KEY(IDB_arrow_left , L"F7 Влево")
	KEY(IDB_arrow_right, L"F8 Clear")
	KEY(IDB_Continue, L"F11 Продолжить")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		//, BUTTON_KEY(IDB_CycleBtn)
		//, BUTTON_KEY(IDB_Continue)
		//, BUTTON_KEY(IDB_Reset)
		//, BUTTON_KEY(IDB_QueryBtn) 		
		//, SeparatorToolbar<1>
#ifdef DEBUG_ITEMS
		, BUTTON_KEY(IDB_arrow_down) 
		, BUTTON_KEY(IDB_arrow_up)
		, BUTTON_KEY(IDB_arrow_left) 
		, BUTTON_KEY(IDB_arrow_right)
		, SeparatorToolbar<2>
#endif
		>::Result tool_button_list;
#undef BUTTON_KEY
//----------------------------------------------------------------------------------
	static bool closed_packet_dialog = true;
	static bool run_once_per_sycle = false;
	void Key<IDB_CycleBtn>::Click(HWND h)
	{
		
	}
	void Key<IDB_Continue>::Click(HWND h)
	{
		//SetEvent(App::ProgrammContinueEvent);
		//AppKeyHandler::Run();
	}
//-------------------------------------------------------------------------------
	void Key<IDB_QueryBtn>::Click(HWND h)
	{
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_down>::Click(HWND h)
	{
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_up>::Click(HWND h)
	{
		zprint("\n");
		PacketWindowDlg(h);
		if(!NumberTubeDlg::Do())Stored::Do();
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_left>::Click(HWND h)
	{
		zprint("\n");	
	}
//------------------------------------------------------------------------------
	void Key<IDB_arrow_right>::Click(HWND h)
	{
		zprint("\n");	
		app.mainWindow.ClearCharts();
	}
//----------------------------------------------------------------------------
	void Key<IDB_Reset>::Click(HWND h)
	{
		//run_once_per_sycle = false;
		//SetEvent(App::ProgrammStopEvent);
		//MainWindow *w = (MainWindow *)GetWindowLong(h, GWL_USERDATA);
		//SetToolButtonText(w->toolBar.hWnd, IDB_CycleBtn, L"F4 Цикл");
		//AppKeyHandler::Stop();
	}
}
//--------------------------------------------------------------------------------------------
HWND MainWindowToolBar::Init(HWND h)
{
	return hWnd = InitToolbar<tool_button_list>()(h);
}
//-------------------------------------------------------------------
void MainWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
//----------------------------------------------------------------------------
const int MainWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;
}
//------------------------------------------------------------------------


