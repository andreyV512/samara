#pragma once
#include "GridNotify.hpp"
#include "LogMessages.h"

class MainWindowGrid
{
public :
	struct Handlers
	{
		void operator()(TCellData &);
		void operator()(TCellColor &);
	};
	HWND hWnd;
	MainWindowGrid();
	GridEventHandler<Handlers> handlers;
	void Init(HWND );
};