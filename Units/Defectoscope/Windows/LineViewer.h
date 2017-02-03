#pragma once
#include "window_tool/message.h"
#include "Graphics/Chart.h"
#include "Graphics\Chart.hpp"
#include "Graphics\ColorLabel.h"
#include "Graphics\BarSeries.h"
#include "Graphics\Borders.h"
#include "DiffApp/App.h"

class LineViewer
{
public:
	Gdiplus::Bitmap *backScreen;
public:
	int offsetX;
public:
	HWND hWnd;
	ColorLabel label;
	Chart *chart;
	Cursor *cursor;	
public:
	TMouseMove storedMouseMove;
	bool mouseMove;
	LineViewer();
	bool CursorDraw(TMouseMove &l, VGraphics &g);
	void operator()(TSize &);
	void operator()(TPaint &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TLButtonDbClk &);
};

template<class T>struct Border: HBorder
{
	Border(Chart &c): HBorder(c){}
};

template<class Thresh>class LineTresholdsViewer: public LineViewer
{
public:	
	typedef LineViewer Parent;
	typedef typename ChartDraw<Chart, typename TL::MultyListToList<typename TL::MkTlst<
		TL::MkTlst<
		LeftAxes
		, BottomAxes
		//, BarSeries
		, Grid	
		, BarSeries
		>::Result
		, typename TL::ListToWapperList<Thresh, Border>::Result
	>::Result>::Result> TChart;
private:
	TChart chartLoc;
	Cursor cursorLoc;
public:
	LineTresholdsViewer()
		: chartLoc(backScreen)
		, cursorLoc(chartLoc)
	{
		chart = &chartLoc;
		cursor = &cursorLoc;
	}
};