#include "stdafx.h"
#include "Windows\ScanWindow.h"
#include "debug_tools/DebugMess.h"
#include "Windows/ViewersMenu.hpp"
#include "window_tool\Emptywindow.h"

using namespace Gdiplus;

ScanWindow::GateIF::GateIF(Chart &c)
	: Gate(c)
	, visible(false)
{
	color = 0xffffff00;
}
void ScanWindow::GateIF::Draw()
{
	if(visible) Gate::Draw();
}

ScanWindow::Gate1::Gate1(Chart &c)
	: Gate(c)
	, visible(false)
{
	color = 0xffff0000;
}
void ScanWindow::Gate1::Draw()
{
	if(visible) Gate::Draw();
}

ScanWindow::Gate2::Gate2(Chart &c)
	: Gate(c)
	, visible(false)
{
	color = 0xff00ffff;
}
void ScanWindow::Gate2::Draw()
{
	if(visible) Gate::Draw();
}

ScanWindow::GateIFBorder::GateIFBorder(Chart &c)
	: VBorder(c)
	, visible(false)
{
	color = 0xffffff00;
}

void ScanWindow::GateIFBorder::Draw()
{
	if(visible) VBorder ::Draw();
}

ScanWindow::Gate1Border::Gate1Border(Chart &c)
	: VBorder(c)
	, visible(false)
{
	color = 0xffff0000;
}

void ScanWindow::Gate1Border::Draw()
{
	if(visible) VBorder::Draw();
}

ScanWindow::Gate2Border::Gate2Border(Chart &c)
	: VBorder(c)
	, visible(false)
{
	color = 0xff00ffff;
}

void ScanWindow::Gate2Border::Draw()
{
	if(visible) VBorder::Draw();
}
//---------------------------------------------------------------------------------
ScanWindow::Line::Line(Chart &c)
	: LineSeries(c)
	, offset(0)
{
	color = 0xff0000ff;
}
void ScanWindow::Line::Draw()
{
	if(NULL != data)
	{
		Color col(color);
		Pen pen(col, 2);
		chart.g->SetClip(&Region(RectF(
			REAL(chart.rect.left + chart.offsetAxesLeft + 1)
			, REAL(chart.rect.top + chart.offsetAxesTop + 1)
			, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 2)
			, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 2)
			)),
			CombineModeReplace
			);
		double dY = (double) (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);

		double yOffs = chart.rect.bottom - chart.offsetAxesBottom;

		int width = chart.rect.right - chart.rect.left - chart.offsetAxesRight - chart.offsetAxesLeft;
		double dX = mash * width /(chart.maxAxesX - chart.minAxesX);
		int x0 = chart.rect.left + chart.offsetAxesLeft;
		double minY = chart.minAxesY;
		int y0 = int(yOffs - (data[0] - minY) * dY);
		double x = x0;
		int y = y0;
		for(int i = offset; i < count; ++i)
		{
			x += dX;
			if(i < 0) 
			{
				y0 = (int)yOffs;
				continue;
			}
			y = int(yOffs - (data[i] - minY) * dY);
			if(x0 != int(x) || y0 != y)
			{
				chart.g->DrawLine(&pen, x0, y0, (int)x, y);
				x0 = int(x);
				y0 = y;
			}
		}
		chart.g->SetClip(&Region());
	}
}

ScanWindow::ScanWindow()
	: chart(backScreen)
{
	chart.minAxesY = 0;
	chart.maxAxesY = 255;
	chart.minAxesX = 0;
	chart.maxAxesX = 512;

	chart.items.get<LineSeries>().data = data;

	label.fontHeight = 12;
}
void ScanWindow::operator()(TSize &l)
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
		toolBar.Size();
		RECT r = {};
		GetClientRect(toolBar.hWnd, &r);
	    if(0 == r.bottom)  return;

		Graphics g(backScreen);
		SolidBrush solidBrush(Color(0xffaaaaaa));
		g.FillRectangle(&solidBrush, 0, r.bottom, l.Width, 20);
		label.top = r.bottom;
		chart.rect.top = r.bottom + 20;
		chart.rect.right = l.Width;
		chart.rect.bottom = l.Height;
		chart.maxAxesY = maxY;
		chart.items.get<LineSeries>().SetData(data, maxX, 0, maxX - 1);
		chart.Draw(g);
		label.Draw(g);
	}
	void ScanWindow::operator()(TPaint &l)
	{
		if(NULL == backScreen) return;
		PAINTSTRUCT p;
		HDC hdc = BeginPaint(l.hwnd, &p);
		{		
			Graphics g(hdc);		
			g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		}
		EndPaint(l.hwnd, &p);
	}
	void ScanWindow::operator()(TCommand &l)
	{
		EventDo(l);
	}
	void ScanWindow::operator()(TGetMinMaxInfo &l)
	{
		if(NULL != l.pMinMaxInfo)
		{
			l.pMinMaxInfo->ptMinTrackSize.x = 300;
			l.pMinMaxInfo->ptMinTrackSize.y = 200;
		}	
	}
	VIEWERS_MENU(ScanWindow)
	unsigned ScanWindow::operator()(TCreate &l)
	{
		mouseMove = false;
		offset = 0;
		Menu<ViewersMenuScanWindow::MainMenu>().Init(l.hwnd);
		toolBar.Init(l.hwnd);
		return 0;
	}
	
	void ScanWindow::operator()(TLButtonDown &l)
	{
		mouseMove = false;
	}

	void ScanWindow::Open(int zone_, int sensor_, int offset_, wchar_t *mess, wchar_t *mess1, USPC7100_ASCANDATAHEADER *uspc, void *o, void(*ptr)())
	{
		if(IsBadReadPtr(uspc, sizeof(uspc))) return;
		owner = o;
		zone = zone_;
		sensor = sensor_;
		offsetInZone = offset_;
		ptrScan = (bool(*)(int, int, int, void *, void(*)()))ptr;		
		for(int i = 0; i < 512; ++i)  data[i] = uspc->Point[i];
		maxX = uspc->DataSize > 0 ?  uspc->DataSize : dimention_of(uspc->Point);
		 maxY = 100;
		 wchar_t buf[1024];
		 wchar_t alarmBuff[128];
		 alarmBuff[0] = 0;
		 if(uspc->CycleAlarm) wcscpy(alarmBuff, L"авария Cycle, ");
		 if(uspc->PrfAlarm) wcscat(alarmBuff, L"авария PRF, ");
		  if(uspc->PowerAlarm) wcscat(alarmBuff, L"авария Power, ");
		 wsprintf(buf, L"%s зона %d датчик %d %s смещение %d номер скана %d", mess, 1 + zone_, 1 + sensor_, alarmBuff, 1 + offset_, uspc->hdr.ScanCounter);
		 
		 label = mess1;
		 HWND h = FindWindow(WindowClass<ScanWindow>()(), 0);
		 if(NULL != h)
		 {			
			 RepaintWindow(h);
			 SetWindowText(h, buf);
			 SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
			 SetForegroundWindow(h);
		 }
		 else
		 {
			 RECT r;
			 WindowPosition::Get<ScanWindow>(r);
			 HWND h = WindowTemplate(this, buf, r.left, r.top, r.right, r.bottom);
			 ShowWindow(h, SW_SHOWNORMAL);
		 }		 
	}

	bool ScanWindow::CursorDraw(TMouseMove &l, VGraphics &g)	  
	{			
		RepaintWindow(l.hwnd);
		return true;
	}

	void ScanWindow::SensPlus() 
	{
		++sensor;
		sensor %= App::count_sensors;
		(*ptrScan)(zone, sensor, offsetInZone, owner, (void(*)())ptrScan);
	}
	void ScanWindow::SensMinus()
	{
		--sensor;
		sensor = (unsigned)sensor % App::count_sensors;
		(*ptrScan)(zone, sensor, offsetInZone, owner, (void(*)())ptrScan);
	}
	void ScanWindow::OffsPlus()	
	{
       ++offsetInZone;
	   if(!(*ptrScan)(zone, sensor, offsetInZone, owner, (void(*)())ptrScan)) --offsetInZone;
	}
	void ScanWindow::OffsMinus()
	{
		--offsetInZone;
	   (*ptrScan)(zone, sensor, offsetInZone, owner, (void(*)())ptrScan);
	}

	void ScanWindow::operator()(TMouseWell &l)
	{
		if(l.delta < 0) OffsPlus(); else OffsMinus();
	}

