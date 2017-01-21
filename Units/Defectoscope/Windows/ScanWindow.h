#pragma once
#include "window_tool/message.h"
#include "Graphics/Chart.h"
#include "Graphics/Chart.hpp"
#include "Graphics/ColorLabel.h"
#include "ScanWindowToolBar.h"
#include "ItemsData/USPCData.h"
#include "Graphics/Gate.h"
#include "Graphics/Borders.h"

class ScanWindow
{
	void *owner;
	bool (*ptrScan)(int, int, int, void *, void(*)());
public:
	class GateIF: public Gate
	{
	public:
		bool visible;
		GateIF(Chart &);
		void Draw();
	};
	class Gate1: public Gate
	{
	public:
		bool visible;
		Gate1(Chart &);
		void Draw();
	};
	class Gate2: public Gate
	{
	public:
		bool visible;
		Gate2(Chart &);
		void Draw();
	};
	class GateIFBorder: public VBorder
	{
	public:
		bool visible;
		GateIFBorder(Chart &);	
		void Draw();
	};
	class Gate1Border: public VBorder
	{
	public:
		bool visible;
		Gate1Border(Chart &);
		void Draw();
	};
	class Gate2Border: public VBorder
	{
	public:
		bool visible;
		Gate2Border(Chart &);
		void Draw();
	}; 
	class Line: public LineSeries
	{
	public:
		double mash;
		int offset;
		Line(Chart &);
		void Draw();
	};
public:
	typedef ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes
		, Line
		, Grid
		, GateIF
		, Gate1		
		, Gate1Border
		, GateIFBorder
		, Gate2Border
		, Gate2
	>::Result>	TChart;
	HWND hWnd;
	TChart chart;
	Gdiplus::Bitmap *backScreen;	
	ScanWindowToolBar toolBar;
	int offset, zone, sensor, offsetInZone;
	int maxX, maxY;
	double data[512];
	TMouseMove storedMouseMove;
	ColorLabel label;
	bool mouseMove;
	ScanWindow();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TLButtonDown &);
	void operator()(TMouseWell &);
	void Open(int zone, int sensor, int offset_, wchar_t *mess, wchar_t *mess1, USPC7100_ASCANDATAHEADER *data, void *, void(*)());
	bool CursorDraw(TMouseMove &, VGraphics &);

	void SensPlus();
	void SensMinus();
	void OffsPlus();
	void OffsMinus();
};
