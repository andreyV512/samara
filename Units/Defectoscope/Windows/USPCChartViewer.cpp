#include "stdafx.h"
#include "USPCChartViewer.h"
#include "EmptyWindow.h"
#include "DebugMess.h"
#include "ConstData.h"
#include "App.h"

using namespace Gdiplus;
//-----------------------------------------------------------------
USPCChartViewer::CursorLabel::CursorLabel(USPCChartViewer &o)
	: owner(o)
	, label(o.label)
	, cursor(o.cursor)
	, chart(o.chart)
{
	label.fontHeight = 10;
	label.top = 2;	
}

bool USPCChartViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	//int x, y;
	//chart.CoordCell(l.x, l.y, x, y);	
	//wsprintf(label.buffer, L"<ff>зона %d  датчик %d        ", 1 + x, 1 + y);
	//label.Draw(g());
	//
	//return x < owner.viewerData.currentOffset;
	return true;
}

bool USPCChartViewer::CursorLabel::GetColorBar(unsigned sensor, int zone, double &data, unsigned &color)
{
	//data = owner.viewerData.buffer[sensor][zone];
	//color = ConstData::ZoneColor(owner.viewerData.status[sensor][zone]);
	//return zone < owner.viewerData.currentOffset;
	return true;
}
//------------------------------------------------------------------
USPCChartViewer::USPCChartViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, cursorLabel(*this)
{
	
	chart.minAxesY = 0;
	chart.maxAxesY = 256;
	chart.minAxesX = 0;
	chart.maxAxesX = 512;
	label.fontHeight = 12;
	label.top = 0;
	chart.rect.top = 17;
	mouseMove = true;
}
//--------------------------------------------------------------------------
void USPCChartViewer::operator()(TSize &l)
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
    Graphics g(backScreen);
	SolidBrush solidBrush(Color(0xffaaaaaa));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  
	
	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;
	chart.Draw(g);

}
//-----------------------------------------------------------------------
void USPCChartViewer::operator()(TPaint &l)
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
void USPCChartViewer::operator()(TMouseMove &l)
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
void USPCChartViewer::operator()(TLButtonDbClk &l)
{
	 mouseMove = true;
	if(cursor.CrossCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
//--------------------------------------------------------------------------------
void USPCChartViewer::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		mouseMove = false;
	
		OffsetToPixel(chart, storedMouseMove.x,  storedMouseMove.y, l.delta / 120, 0 == l.flags.lButton); 
		cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
}
//--------------------------------------------------------------------------------------
void USPCChartViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
//-----------------------------------------------------------------------------------------
void USPCChartViewer::Update()
{
	RepaintWindow(hWnd);
}
//------------------------------------------------------------------------------------------------
unsigned USPCChartViewer::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.x = 0;	
	storedMouseMove.y = WORD(chart.rect.top + 1);
	StartTimer();
	return 0;
}
//-------------------------------------------------------------------------------------
void USPCChartViewer::operator()(TTimer &l)
{
	//test	 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	double *d = data;
	int i = 0;
	for(; i < 10; ++i)
	{
		d[i] = rand() % 40;
	}
	for(; i < 20; ++i)
	{
		d[i] = rand() % 200;
	}
	for(; i < 200; ++i)
	{
		d[i] = rand() % 35;
	}
	for(; i < 220; ++i)
	{
		d[i] = rand() % 160;
	}
	for(; i < 400; ++i)
	{
		d[i] = rand() % 30;
	}
	for(; i < 420; ++i)
	{
		d[i] = rand() % 120;
	}
	for(; i < dimention_of(data); ++i)
	{
		d[i] = rand() % 20;
	}
	//test	##########################################################
	chart.items.get<LineSeries>().SetData(data, 512, 0, 511);
	Update();
}
//-------------------------------------------------------------------------------------
void USPCChartViewer::operator()(TDestroy &m)
{
	StopTimer();
	delete backScreen;
    backScreen = NULL;
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
}
//-------------------------------------------------------------------------------------
void USPCChartViewer::StartTimer()
{
   SetTimer(hWnd, timerID, 500, NULL);
}
void USPCChartViewer::StopTimer()
{
	KillTimer(hWnd, timerID);
}
//--------------------------------------------------------------------------------------

