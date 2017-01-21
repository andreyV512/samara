#pragma once

#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "debug_tools/DebugMess.h"
#include "window_tool\Emptywindow.h"

namespace AxesSpace
{
template<template<class>class Wapper, class Z, class T>struct Skip<Wapper<AxesYMin<T> >, Z>
{
	template<class P>bool operator()(Wapper<AxesYMin<T> > *, P *){return true;}
};
template<template<class>class Wapper, class Z, class T>struct Skip<Wapper<AxesYMax<T> >, Z>
{
	template<class P>bool operator()(Wapper<AxesYMax<T> > *, P *){return true;}
};

template<class T>struct ParamTitle<AxesYMin<T> >
{
   wchar_t *operator()(){return L"Ось Y минимальное значение";}
};

template<class T>struct ParamTitle<AxesYMax<T> >
{
   wchar_t *operator()(){return L"Ось Y максимальное значение";}
};

template<class T>struct AxesYDlg
{
	static void Do(HWND h)
	{
		if(TemplDialogList<ParametersBase, AxesTable
			, TL::SelectT<typename AxesTable::items_list
			   , T>::Result>(Singleton<AxesTable>::Instance()).Do(h, L"Ось Y"))
		{
			RepaintWindow(h);
		}
	}
};
}

