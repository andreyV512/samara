#pragma once
#include "LongViewer.h"
#include "LineViewer.h"
#include "templates/typelist.hpp"
#include "ItemsData\DataViewer.h"
#include "ItemsData/ConstData.h"
#include "LineChart.hpp"
#include "Common.h"

class LongWindow
{
public:
	typedef Long sub_type;
	int lastZone;
public:
	HWND hWnd;
	bool drawZones;
	
	typedef TL::CreateWapperNumList<Line, LongWindow, 0, 7>::Result line_list;
	typedef TL::AppendList<
		    TL::MkTlst<NoSubMenu<LongViewer> >::Result
		    , line_list
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	LongViewer &viewer;
	LongWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);
};