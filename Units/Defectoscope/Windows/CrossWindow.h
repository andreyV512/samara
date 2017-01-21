#pragma once
#include "CrossViewer.h"
#include "LineViewer.h"
#include "templates/typelist.hpp"
#include "ItemsData\DataViewer.h"
#include "ItemsData/ConstData.h"
#include "LineChart.hpp"
#include "Common.h"

class CrossWindow
{
public:
	typedef Cross sub_type;
	int lastZone;
public:
	HWND hWnd;
	bool drawZones;
	
	typedef TL::CreateWapperNumList<Line, CrossWindow, 0, App::count_sensors - 1>::Result line_list;
	typedef TL::AppendList<
		    TL::MkTlst<NoSubMenu<CrossViewer>>::Result
		    , line_list
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	CrossViewer &viewer;
	CrossWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);	
};