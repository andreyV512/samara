#pragma once
#include "window_tool/SelectTypeSizeList.h"
#include <windows.h>
#include "window_tool/message.h"
#include "Windows/TopLabelViewer.h"
#include "window_tool/CheckBoxWidget.h"
#include "MainWindowToolBar.h"
#include <CommCtrl.h>
#include "Windows/ThicknessViewer.h"
#include "Windows/CrossViewer.h"
#include "Windows/LongViewer.h"
#include "Windows/ResultViewer.h"
//#include "MessagesInterface.h"

template<class T, class Parent>struct OnTheJobCheckBox
{
protected:
	void Command(TCommand &m, bool b)
	{
		Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<T> >().value = b;
		Parent::CheckBoxStateStoreInBase();
	}
	bool Init(HWND h)
	{		
		return Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<T> >().value;
	}
};

//o->select.AddMenuItem(buf);
struct PlugSelect
{
	template<class T>void AddMenuItem(T &){}
};

class MainWindow
{
public:
	HWND hWnd;
	HWND hStatusWindow;
	MainWindowToolBar toolBar;
	CheckBoxWidget<OnTheJobCheckBox<Cross    , MainWindow> > crossCheckBox;
	CheckBoxWidget<OnTheJobCheckBox<Long     , MainWindow> > longCheckBox;
	CheckBoxWidget<OnTheJobCheckBox<Thickness, MainWindow> > thicknessCheckBox;
	TopLabelViewer topLabelViewer;
	typedef TL::MkTlst<
		CrossViewer
		, LongViewer
		, ThicknessViewer
		, ResultViewer
	>::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	PlugSelect select;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TMouseWell &);
	void operator()(TUser &);
	void operator()(TClose &);
	void operator()(TCopyData &);
	static void CheckBoxStateStoreInBase();

	void ClearCharts();
	void UpdateCheck();
};
