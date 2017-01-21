#pragma once
#include "window_tool\GridNotify.hpp"

class LogUSPCWindow;
class LogUSPCWindowGrid
{
public :
	struct Handlers
	{
		void operator()(TCellData &);
		void operator()(TCellColor &);
	};
	HWND hWnd;
	LogUSPCWindowGrid();
	GridEventHandler<Handlers> handlers;
	void Init(HWND );
};