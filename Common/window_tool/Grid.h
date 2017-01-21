#pragma once
//#include "window_tool\GridItems.h"
#include <Windows.h>
#include <CommCtrl.h>
#include "window_tool\GridItems.h"
#include "window_tool/message.h"
template<class T>class SetGridHeader
{
	HWND hWnd;
	LV_COLUMN lvc;
	template<class O, class P>struct loc
	{
		void operator()(P *p)
		{
			p->lvc.iSubItem = TL::IndexOf<T, O>::value;
			p->lvc.pszText = header_table<O>().name();
			p->lvc.cx = header_table<O>::width;
			ListView_InsertColumn(p->hWnd, p->lvc.iSubItem, &p->lvc);
		}
	};
public:
	explicit SetGridHeader(HWND hWnd) : hWnd(hWnd) 
	{
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.fmt = LVCFMT_LEFT;
		TL::foreach<T, loc>()(this);		
	}	
};

struct HeaderData
{
	wchar_t *name;
	int width;
};

void SetGridHeaderArray(HWND, HeaderData *, int);

class GridNotify
{	
public:
	HWND hWnd;
	GridHandlers *handlers;
	~GridNotify();
	void Size(int, int, int, int);
	void Create(TCreate &, GridHandlers *);
	unsigned Notify(TNotify &);
	static unsigned OwnerNotify(TNotify &);
};

