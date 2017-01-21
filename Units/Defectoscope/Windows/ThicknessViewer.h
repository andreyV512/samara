#pragma once
#include "window_tool/message.h"
#include "Graphics/ColorLabel.h"
#include "Graphics/Chart.h"
#include "Graphics/Chart.hpp"
#include "Graphics/BarSeries.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/GridChart.h"
#include "Common.h"
#include "ItemsData/USPCData.h"

class ThicknessViewer
{
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		NoOffsetLeftAxes
		, BottomAxesGrid
		, BarSeriesDouble
		, FixedGrid		
	>::Result> TChart;
	TChart chart;
	bool mouseMove;
	Gdiplus::Bitmap *backScreen;
	ColorLabel label;
	Cursor cursor;
	int currentX, currentY;
	bool painting;
public:
	HWND hWnd;
	TMouseMove storedMouseMove;
	bool openDetailedWindow;
	ItemData<Thickness> &viewerData;
	ThicknessViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &l);
	void operator()(TRButtonDown &);

	bool Draw(TMouseMove &, VGraphics &);
	bool GetColorBar(int , double &, unsigned &, double &, unsigned &);
};

