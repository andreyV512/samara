#pragma once

#include "window_tool/message.h"
#include "Graphics/Chart.h"
#include "Graphics/Chart.hpp"
#include "Graphics/GridChart.h"
#include "Graphics/ColorLabel.h"
#include "Graphics/BarSeries.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/StepSeries.h"
#include "DiffApp/App.h"
#include "window_tool/CheckBoxWidget.h"
#include "window_tool/RadioBtnWidget.h"

class ThicknessThresholdWindow
{
public:
	void AlignOneZone (int, double);
	void AlignAllZones(int, double);
	void AlignThresh  (int, double);
	class CbHandler
	{
	public:
		CbHandler(): value(true){}
		bool value;
		void Command(TCommand &m, bool b)
		{
			value = b;
		}
		bool Init(HWND h){return value;}
	};
	template<bool value>class FastOffset
	{
	public:
		ThicknessThresholdWindow &owner;
		template<class T>FastOffset(T &t): owner(t){}
		void Command(TCommand &m)
		{
			owner.fastOffset = value;
			SetFocus(owner.hWnd);
		}
		bool Init(HWND h, bool initValue){return initValue;}
	};
	template<void(ThicknessThresholdWindow::*ptr)(int, double)>class AlignThreshold
	{
	public:
		ThicknessThresholdWindow &owner;
		template<class T>AlignThreshold(T &t): owner(t){}
		void Command(TCommand &m)
		{
			owner.ptrAlign = ptr;
		}
		bool Init(HWND h, bool initValue){return initValue;}
	};
	template<>class AlignThreshold<&ThicknessThresholdWindow::AlignAllZones>
	{
	public:
		ThicknessThresholdWindow &owner;
		template<class T>AlignThreshold(T &t): owner(t){}
		void Command(TCommand &m);		
		bool Init(HWND h, bool initValue){return initValue;}
	};
public:
	struct AboveThresh  : StepSeries{AboveThresh  (Chart &c): StepSeries(c){}};
	struct LowerThresh  : StepSeries{LowerThresh  (Chart &c): StepSeries(c){}};
	struct NominalThresh: StepSeries{NominalThresh(Chart &c): StepSeries(c){}};
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesGrid
		, AboveThresh  
		, LowerThresh  
		, NominalThresh
		, FixedGrid
	>::Result>	TChart;

	HWND hWnd;
	HWND hToolBar;

	void(ThicknessThresholdWindow::*ptrAlign)(int, double);
	
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	bool mouseMove;
	bool fastOffset;
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
	double &minAxesY;
	double &maxAxesY;
	double aboveBorder   [App::count_zones];
	double lowerBorder   [App::count_zones];
	double nominalBorder [App::count_zones];
public:
	CheckBoxWidget<CbHandler> aboveBorderCheckBox  ;
	CheckBoxWidget<CbHandler> lowerBorderCheckBox  ;
	CheckBoxWidget<CbHandler> nominalBorderCheckBox;

	RadioBtnWidget<FastOffset<false> > offset01;
	RadioBtnWidget<FastOffset<true> > offset50;	

	RadioBtnWidget<AlignThreshold<&ThicknessThresholdWindow::AlignOneZone> > alignOneZone;
	RadioBtnWidget<AlignThreshold<&ThicknessThresholdWindow::AlignAllZones> > alignAllZones;
	RadioBtnWidget<AlignThreshold<&ThicknessThresholdWindow::AlignThresh> > alignThreshold;
	ThicknessThresholdWindow();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TKeyDown &);
	void operator()(TCommand &);

	bool Draw(TMouseMove &, VGraphics &);

	void Draw(TSize &);

	void operator()(TClose &);
	static wchar_t *Title();

	void operator()(TDestroy &);
};