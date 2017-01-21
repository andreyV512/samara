#pragma once
#include "window_tool/message.h"
#include "Graphics/Chart.h"
#include "Graphics/Chart.hpp"
#include "Graphics/GridChart.h"
#include "Graphics/ColorLabel.h"
#include "Graphics/BarSeries.h"
#include "Graphics/FixedGridSeries.h"
#include "ItemsData/USPCData.h"

class LongViewer
{
public:
	HWND hWnd;
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		FixedLeftAxes
		, BottomAxesGrid
		, FixedGridSeries
		, FixedGrid		
	>::Result> TChart;
	Gdiplus::Bitmap *backScreen;
	TChart chart;
	bool mouseMove;
public:
	TMouseMove storedMouseMove;
	ColorLabel label;
public:
	Cursor cursor;
	int currentX, currentY;
	ItemData<Long> &viewerData;
public:
	LongViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);

	bool LongViewer::Draw(TMouseMove &, VGraphics &);
	bool LongViewer::GetColorBar(unsigned , int , double &, unsigned &);
};