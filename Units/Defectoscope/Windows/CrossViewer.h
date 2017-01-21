#pragma once
#include "window_tool/message.h"
#include "Graphics\Chart.h"
#include "Graphics\Chart.hpp"
#include "Graphics\GridChart.h"
#include "Graphics\ColorLabel.h"
#include "Graphics\BarSeries.h"
#include "Graphics\FixedGridSeries.h"
#include "ItemsData/USPCData.h"
class CrossViewer
{
public:
	HWND hWnd;
	typedef ChartDraw<Chart, TL::MkTlst<
		FixedLeftAxes
		, BottomAxesGrid
		, FixedGridSeries
		, FixedGrid
	>::Result>	TChart;
public:
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	bool mouseMove;
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
	int currentX, currentY;
public:
	ItemData<Cross> &viewerData;
	CrossViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);

	bool Draw(TMouseMove &, VGraphics &);
	bool GetColorBar(unsigned , int , double &, unsigned &);
};