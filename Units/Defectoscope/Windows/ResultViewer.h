#pragma once
#include "window_tool/message.h"
#include "Graphics/ColorLabel.h"
#include "Graphics/Chart.h"
#include "ItemsData\ResultData.h"
#include "Graphics/Chart.hpp"
#include "Graphics/BarSeries.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/EmptyLeftAxes.h"

class ResultViewer
{
	class BottomAxesMeters__: public BottomAxesMeters
	{
	public:
		BottomAxesMeters__(Chart &c): BottomAxesMeters(c, App::zone_length){}
	};

	class CancelOperatorSeries
	{
	public:
		BarSeries barSeries;
		CancelOperatorSeries(Chart &c);//: barSeries(c){}
		void Draw();
	};

public:
	typedef ChartDraw<Chart, TL::MkTlst<
		EmptyLeftAxes
		, BottomAxesMeters__
		, BarSeries
		, CancelOperatorSeries
		, FixedGrid		
	>::Result> TChart;
	TChart chart;
	bool mouseMove;
	Gdiplus::Bitmap *backScreen;
	ColorLabel label;
	Cursor cursor;
	bool painting;
	int currentX;
public:
	HWND hWnd;
	TMouseMove storedMouseMove;
	bool openDetailedWindow;
	ResultViewerData &viewerData;
	ResultViewer();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &l);
	void operator()(TDestroy &);

	bool Draw(TMouseMove &, VGraphics &);
	bool GetColorBar(int , double &, unsigned &);
	bool ResultViewer::GetColorBarCancelOperator(int zone, double &data, unsigned &color);
};

