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

class TresholdWindow
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
		TresholdWindow &owner;
		template<class T>FastOffset(T &t): owner(t){}
		void Command(TCommand &m)
		{
			owner.fastOffset = value;
			SetFocus(owner.hWnd);
		}
		bool Init(HWND h, bool initValue){return initValue;}
	};
	template<void(TresholdWindow::*ptr)(int, double)>class AlignThreshold
	{
	public:
		TresholdWindow &owner;
		template<class T>AlignThreshold(T &t): owner(t){}
		void Command(TCommand &m)
		{
			owner.ptrAlign = ptr;
		}
		bool Init(HWND h, bool initValue){return initValue;}
	};
	template<>class AlignThreshold<&TresholdWindow::AlignAllZones>
	{
	public:
		TresholdWindow &owner;
		template<class T>AlignThreshold(T &t): owner(t){}
		void Command(TCommand &m);		
		bool Init(HWND h, bool initValue){return initValue;}
	};
public:
	struct BrackThresh: StepSeries{BrackThresh(Chart &c): StepSeries(c){}};
	struct Klass2Thresh: StepSeries{Klass2Thresh(Chart &c): StepSeries(c){}};
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesGrid
		, BrackThresh
		, Klass2Thresh
		, FixedGrid
	>::Result>	TChart;

	HWND hWnd;
	HWND hToolBar;

	void(TresholdWindow::*ptrAlign)(int, double);
	
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	bool mouseMove;
	bool fastOffset;
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
	double brak[App::count_zones];
	double klass2[App::count_zones];
public:
	CheckBoxWidget<CbHandler> border2ClassCheckBox;
	CheckBoxWidget<CbHandler> borderDefectCheckBox;

	RadioBtnWidget<FastOffset<false> > offset01;
	RadioBtnWidget<FastOffset<true> > offset50;	

	RadioBtnWidget<AlignThreshold<&TresholdWindow::AlignOneZone> > alignOneZone;
	RadioBtnWidget<AlignThreshold<&TresholdWindow::AlignAllZones> > alignAllZones;
	RadioBtnWidget<AlignThreshold<&TresholdWindow::AlignThresh> > alignThreshold;
	TresholdWindow();
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
	void operator()(TDestroy &);
};