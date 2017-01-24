#include "stdafx.h"
#include "Windows/ThicknessViewer.h"
#include <stdio.h>
#include "debug_tools/DebugMess.h"
#include "MessageText\SelectMessage.h"
#include "debug_tools/DebugMess.h"
#include "App/AppBase.h"
#include "DiffApp/App.h"
#include "ThicknessWindow.h"
#include "ViewersMenu.hpp"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

bool ThicknessViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x, y;
	chart.CoordCell(l.x, l.y, x, y);
	
	bool drawZones =  x < viewerData.currentOffsetZones;
	if(drawZones)
	{
	    currentX = x;
		int color;
		bool b;
		char *s = StatusText()(viewerData.commonStatus[x], color, b);
		wchar_t buf[128];
		if(b)
		{
			char min[128], max[128];
			sprintf(min, "%.2f",viewerData.bufferMin[x]);
			sprintf(max, "%.2f",viewerData.bufferMax[x]);

			for(char *s = min; *s; ++s){if('.' == *s){s += 2;*s = '\0';break;}}
			for(char *s = max; *s; ++s){if('.' == *s){s += 2;*s = '\0';break;}}
			
			wsprintf(buf, L"<ff>мин.толщина <ff0000>%S <ff>мах.толщина <ff0000>%S", 
				min
				, max
				);
		}
		else
		{
			buf[0] = 0;
		}
		wsprintf(label.buffer, L"<ff>Толщина зона %d  <%6x>%S %s"
			, 1 + x
			, color
			, s
			, buf
			);
		label.Draw(g());
	}
	return drawZones;
}

bool ThicknessViewer::GetColorBar(int zone, double &data_, unsigned &color, double &data_1, unsigned &color1)
{
	data_1 = viewerData.bufferMin[zone];
	data_ =  viewerData.bufferMax[zone];
	if(MinAndLamination(viewerData.commonStatus[zone]))
	{
		color1 = Singleton<ColorTable>::Instance().items.get<Clr<BorderLower<Thickness> >>().value;
		color = Singleton<ColorTable>::Instance().items.get<Clr<BrakStrobe2<Thickness> >>().value;
	}
	else
	{
		ColorBar()(
			data_1
			, color1
			, viewerData.commonStatus[zone]
		, Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<Thickness> >().value[zone]
		);
		color = color1;
		unsigned char *x = (unsigned char *) &color;
		x[0] = unsigned char(2.5 * x[0] / 4);
		x[1] = unsigned char(2.5 * x[1] / 4);
		x[2] = unsigned char(2.5 * x[2] / 4);
	}
	return zone < viewerData.currentOffsetZones;	  
}
//-----------------------------------------------------------------------------
ThicknessViewer::ThicknessViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, openDetailedWindow(false)
	, painting(true)
	, mouseMove(true)
	, viewerData(Singleton<ItemData<Thickness> >::Instance())
{
	chart.rect.top = 17;
	
	chart.minAxesX = 0;
	chart.maxAxesX = App::count_zones;
	chart.minAxesY = 0;

	label.fontHeight = 12;
	label.top = 0;	

	cursor.SetMouseMoveHandler(this, &ThicknessViewer::Draw);
	chart.items.get<BarSeriesDouble>().SetColorBarHandler(this, &ThicknessViewer::GetColorBar);
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void ThicknessViewer::operator()(TSize &l)
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
	g.FillRectangle(&solidBrush, 0, 29, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  

	chart.minAxesY = Singleton<AxesTable>::Instance().items.get<AxesYMin<Thickness> >().value;
	chart.maxAxesY = Singleton<AxesTable>::Instance().items.get<AxesYMax<Thickness> >().value;
	chart.Draw(g);
}
//----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TPaint &l)
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
//-----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}
//----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
//--------------------------------------------------------------------------
void ThicknessViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
void ThicknessViewer::operator()(TMouseWell &l)
{	
		mouseMove = false;

		OffsetToPixel(
			chart
			, storedMouseMove.x
			, storedMouseMove.y
			, l.delta / 120
			, true 
			);
		cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));

		chart.CoordCell(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
}
//------------------------------------------------------------------------------------------------------
unsigned ThicknessViewer::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	currentX = currentY = 0;
	storedMouseMove.x = WORD((chart.rect.right - chart.rect.left) / 2);	
	storedMouseMove.y = WORD((chart.rect.bottom - chart.rect.top) / 2);
	return 0;
}
//------------------------------------------------------------------------------------------
CONTEXT_MENU(ThicknessWindow)
void ThicknessViewer::operator()(TRButtonDown &l)
{
	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thickness>>().value)
	PopupMenu<ContextMenuThicknessWindow::items_list>::Do(l.hwnd, l.hwnd);
}
//--------------------------------------------------------------------------------