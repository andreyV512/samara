#include "stdafx.h"
#include "MainWindow.h"
#include "window_tool/MenuApi.h"
#include "MainWindowMenu.hpp"
#include "window_tool\EmptyWindow.h"
#include <CommCtrl.h>
#include "Windows/Common.h"
#include "Automat/AppKeyHandler.h"

#include "debug_tools/DebugMess.h"
namespace {
	static const int resultViewerHeight = 100;
	struct __move_window_data__
	{
		int y, width, height;
	};
	template<class O, class P>struct __move_window__
	{
		void operator()(O *o, P *p)
		{			
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, p->height};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, p->height, true);
			p->y += p->height;
		}
	};

	template<class P>struct __move_window__<ResultViewer, P>
	{
		typedef ResultViewer O;
		void operator()(O *o, P *p)
		{
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, resultViewerHeight};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, resultViewerHeight, true);
		}
	};
}
//------------------------------------------------------------------------
void MainWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();

	RECT rt;
	GetWindowRect(toolBar.hWnd, &rt);
    RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(m.hwnd, &r);	

	static const int width = toolBar.Width();
	select.Size(width, 5, 400);

	//testCheckBox0.Size(width + 525, 52, 200, 20);
	//testCheckBox1.Size(width, 52, 525, 20);
	//testCheckBox2.Size(width, 69, 525, 20);

	crossCheckBox	 .Size (width + 425, 5 , 400, 20);
	longCheckBox	 .Size (width + 425, 25, 400, 20);
	thicknessCheckBox.Size (width + 425, 45, 400, 20);
	viewInterruptCheckBox.Size(width, 55, 400, 20);

	static const int topLabelHeight = 28;
	int y = rt.bottom - rt.top - 1;
	int t = r.bottom - rs.bottom - rt.bottom + rt.top + 2 - topLabelHeight - resultViewerHeight;
	MoveWindow(topLabelViewer.hWnd , 0, y, r.right, topLabelHeight, true);
	y += topLabelHeight;
	t /= 3;
	__move_window_data__ data = {y, r.right, t};
	
	TL::foreach<viewers_list, __move_window__>()(&viewers, &data);
	//topLabelViewer.SetMessage(L"<ff>Test <ff00>message <ff0000>TopLabel");
}
//------------------------------------------------------------------------
void MainWindow::operator()(TCommand &m)
{
	EventDo(m);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
	}		
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TCreate &m)
{
	Menu<MainWindowMenu::MainMenu>().Init(m.hwnd);
//
	toolBar.Init(m.hwnd);
	select.Create(toolBar.hWnd);
//
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {200, 400, 600, 900};
	SendMessage(hStatusWindow, SB_SETPARTS, dimention_of(pParts), (LPARAM)pParts);
////	//test
//    testCheckBox0.Init(toolBar.hWnd, L"test check box 0");
//    testCheckBox1.Init(toolBar.hWnd, L"test check box 1");
//    testCheckBox2.Init(toolBar.hWnd, L"test check box 2");
////	//test end
////

	crossCheckBox	 .Init(toolBar.hWnd, L"Измерение поперечных дефектов стенки трубы");
	longCheckBox	 .Init(toolBar.hWnd, L"Измерение продольных дефектов стенки трубы");
	thicknessCheckBox.Init(toolBar.hWnd, L"Измерение толщины стенки трубы");
	viewInterruptCheckBox.Init(toolBar.hWnd, L"Прерывание на просмотр");

	topLabelViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<TopLabelViewer>::Proc, L"TopLabelWindow", &topLabelViewer);
	topLabelViewer.label.fontHeight = 16;
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &m.hwnd);
	return 0;
}
//-------------------------------------------------------------------------
//void MainWindow::operator()(TRButtonDown &l)
//{
//  typedef TL::EraseItem<viewers_list, ResultViewer>::Result lst;
//  TL::find<lst, Common::__in_rect__>()(
//	  &viewers
//	  , &Common::__event_data__<TRButtonDown, MainWindow>(*this, l)
//	  );
//}
//------------------------------------------------------------------------
void MainWindow::operator()(TDestroy &)
{
	PostQuitMessage(0);
}
void MainWindow::operator()(TClose &l)
{
	if(IDYES == MessageBox(l.hwnd, L"Выйти из программы?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
	{
		DestroyWindow(l.hwnd);
	}
}
//---------------------------------------------------------------------------
typedef void(*TptrMess)(void *);
void MainWindow::operator()(TMessage &m)
{
	if(m.wParam)((TptrMess )(m.wParam))((void *)m.lParam);
}
//-----------------------------------------------------------------------------
namespace Common
{
	template<class P>struct __in_rect__<ResultViewer, P>: __in_rect_all__<ResultViewer, P, MainWindow::viewers_list>{};
}
//------------------------------------------------------------------------------
void MainWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, MainWindow>(*this, l)
		);
}
//--------------------------------------------------------------------------------
void MainWindow::operator()(TUser &l)
{
	(*l.ptr)(l.data);
}
//--------------------------------------------------------------------------------
void MainWindow::CheckBoxStateStoreInBase()
{
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		UpdateWhere<OnTheJobTable>(Singleton<OnTheJobTable>::Instance(), base).ID(1).Execute();
	}
}
//------------------------------------------------------------------
namespace
{
	template<class O, class P>struct __clear__
	{
		void operator()(O &o)
		{
			o.viewerData.currentOffsetZones = 0;
		}
	};
}
void MainWindow::ClearCharts()
{
	TL::foreach<viewers_list, __clear__>()(viewers);
	RepaintWindow(hWnd);
}



