#include "stdafx.h"
#include "CrossViewer.h"
#include "debug_tools/DebugMess.h"
#include "debug_tools/DebugMess.h"
#include "ItemsData/ConstData.h"
#include "window_tool/MenuApi.h"
#include "ViewersMenu.hpp"
#include "CrossWindow.h"
#include "Common.h"
#include "MessageText\SelectMessage.h"

using namespace Gdiplus;
//-----------------------------------------------------------------------------------------
CrossViewer::CrossViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, viewerData(Singleton<ItemData<Cross> >::Instance())
{
	chart.minAxesY = 1;
	chart.maxAxesY = 1 + App::count_sensors;
	chart.minAxesX = 0;
	chart.maxAxesX = App::count_zones;
	chart.rect.top = 17;
	mouseMove = true;
	label.fontHeight = 12;
	label.top = 0;
	chart.items.get<FixedGridSeries>().SetColorCellHandler(this, &CrossViewer::GetColorBar);
	cursor.SetMouseMoveHandler(this, &CrossViewer::Draw);	
	chart.items.get<FixedGridSeries>().sensorCount = App::count_sensors;
	zprint("\n");
}
//--------------------------------------------------------------------------
bool CrossViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x, y;
	chart.CoordCell(storedMouseMove.x, storedMouseMove.y, x, y);
	

	if(y >= App::count_sensors) return false;

	bool drawZones =  x < viewerData.currentOffsetZones;
	if(drawZones)
	{		
		int color;
		bool b;
		char *s = StatusText()(viewerData.status[y][x], color, b);
		wchar_t buf[128];
		if(b)
		{
			wsprintf(buf, L"<ff>значение <ff0000>%s", Wchar_from<double>(viewerData.buffer[y][x])());
		}
		else
		{
			buf[0] = 0;
		}
		wsprintf(label.buffer, L"<ff>Поперечный зона %d  датчик %d   <%6x>%S %s"
			, 1 + x
			, 1 + y
			, color
			, s
			, buf
			);
		label.Draw(g());
	}
	return drawZones;
}
bool CrossViewer::GetColorBar(unsigned sensor, int zone, double &data, unsigned &color)
{
	--sensor;
	data = viewerData.buffer[sensor][zone];
	color = ConstData::ZoneColor(viewerData.status[sensor][zone]);
	return zone < viewerData.currentOffsetZones;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void CrossViewer::operator()(TSize &l)
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

	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;

	ZoneToCoord(chart, currentX, currentY, storedMouseMove.x, storedMouseMove.y);

    Graphics g(backScreen);
	SolidBrush solidBrush(Color(0xffaaaaaa));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  
	
	chart.Draw(g);	
}
//-----------------------------------------------------------------------
void CrossViewer::operator()(TPaint &l)
{
	if(NULL == backScreen) return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor.CrossCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}
//------------------------------------------------------------------------
void CrossViewer::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(cursor.CrossCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}
//------------------------------------------------------------------------------
void CrossViewer::operator()(TLButtonDbClk &l)
{
	 mouseMove = true;
	if(cursor.CrossCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
//--------------------------------------------------------------------------------
void CrossViewer::operator()(TMouseWell &l)
{
		mouseMove = false;
	
		chart.items.get<FixedGridSeries>().OffsetToPixel(
			storedMouseMove.x
			, storedMouseMove.y
			, l.delta / 120
			, 0 == l.flags.lButton 
			);
		cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));	

		chart.CoordCell(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
}
//--------------------------------------------------------------------------------------
void CrossViewer::operator()(TLButtonDown &)
{
		mouseMove = false;
}
//------------------------------------------------------------------
unsigned CrossViewer::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	currentX = currentY = 0;
	storedMouseMove.x = WORD((chart.rect.right - chart.rect.left) / 2);	
	storedMouseMove.y = WORD((chart.rect.bottom - chart.rect.top) / 2);
	return 0;
}
//------------------------------------------------------------------------------------------
CONTEXT_MENU(CrossWindow)
void CrossViewer::operator()(TRButtonDown &l)
{
	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Cross>>().value)
	PopupMenu<ContextMenuCrossWindow::items_list>::Do(l.hwnd, l.hwnd);
}
//--------------------------------------------------------------------------------

