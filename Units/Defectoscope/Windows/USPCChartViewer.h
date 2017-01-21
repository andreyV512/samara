#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "GridChart.h"
#include "ColorLabel.h"
#include "BarSeries.h"
#include "FixedGridSeries.h"
#include "Gate.h"

template<class T>class GateItem;
struct gateIF;
struct gate1;
struct gate2;
template<>class GateItem<gateIF>: public Gate
{
public:
	GateItem(Chart &c): Gate(c)
	{
		color = 0xff0000ff;
	}
};
template<>class GateItem<gate1>: public Gate
{
public:
	GateItem(Chart &c): Gate(c)
	{
		color = 0xff00ff00;
	}
};
template<>class GateItem<gate2>: public Gate
{
public:
	GateItem(Chart &c): Gate(c)
	{
		color = 0xffff0000;
	}
};

class USPCChartViewer
{
	static const int timerID = 123456;
	double data[512];
public:
	HWND hWnd;
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes
		, LineSeries		
		, Grid	
		, GateItem<gateIF>
		, GateItem<gate1>
		, GateItem<gate2>
	>::Result> TChart;
	struct CursorLabel
	{
		friend class USPCChartViewer;
		USPCChartViewer &owner;
		ColorLabel &label;
		Cursor &cursor;
		TChart &chart;
		CursorLabel(USPCChartViewer &);
		bool Draw(TMouseMove &l, VGraphics &g);
		bool GetColorBar(unsigned sensor, int zone, double &data, unsigned &color);
	};
	Gdiplus::Bitmap *backScreen;
	TChart chart;
	bool mouseMove;
public:
	TMouseMove storedMouseMove;
private:
	ColorLabel label;
	Cursor cursor;
	CursorLabel cursorLabel;
public:
	USPCChartViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TTimer &);
	void operator()(TDestroy &);
	void Update();
	void StartTimer();
	void StopTimer();	
};