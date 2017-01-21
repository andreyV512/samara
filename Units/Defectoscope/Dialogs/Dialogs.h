#pragma once
#include <windows.h>
#include "templates/typelist.hpp"
//-----------------------------------------------------------------
class CopyFactory
{
template<class O, class P>struct loc
{
	void operator ()(O *o, P *p)
	{
		o->value = p->get<O>().value;
	}
};
public:
	template<class List>static void Do(TL::Factory<List> &to, TL::Factory<List> &from)
	{
		TL::foreach<List, loc>()(&to, &from);
	}
};
//-------------------------------------------------------------------
#define DLG(n)struct n{static void Do(HWND);};

DLG(Descriptor1730Dlg)
DLG(OutputsDlg)
DLG(InputsDlg)
DLG(AddTypeSizeDlg)
DLG(DelTypeSizeDlg)
DLG(DeadZonesDlg)
DLG(AboutWindowDlg)
DLG(IOportsDlg)
DLG(TestTabsDlg)
DLG(RecalculationDlg)
DLG(MedianFiltreDlg)
DLG(ColorItemsDlg)

DLG(OffsetsSensorsDlg)

DLG(StoredDataDlg)
DLG(LoadDataDlg)

DLG(AnalogPlateDlg)
DLG(BaseLengthDlg)

DLG(BrackStrobe2Dlg)
DLG(ProtectiveThickeningDlg)

#undef DLG

template<class T>struct WindowPositionDlg
{
	static void Do(HWND h)
	{
		RECT r;
		GetWindowRect(h, &r);
		WindowPosition::Set<T>(r);
	}
};