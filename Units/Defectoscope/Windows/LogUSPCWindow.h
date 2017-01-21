#pragma once
#include "window_tool/message.h"
#include "LogUSPCWindowGrid.h"
#include "USPC/LogUSPC.h"

class LogUSPCWindow
{
	int lastIndex;
	LogUSPCWindowGrid mainWindowGrid;
public:
	LogUSPC data;
public:
	HWND hWnd;
	LogUSPCWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	unsigned operator()(TNotify &);
	static void Open();
};
