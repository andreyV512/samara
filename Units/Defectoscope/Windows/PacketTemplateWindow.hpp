#pragma once

#include "debug_tools/DebugMess.h"
#include "ViewersMenu.hpp"
#include "Stored/PacketBase.h"
#include "window_tool\MessagesInterface.h"
#include "GridWindow.h"
#include "window_tool\Grid.h"
#include "GridData/PacketTemplateData.hpp"

template<class Table, class Col>class PacketTemplateWindow
{
public:
	class OkBtn: public TEvent
	{
	public:
		PacketTemplateWindow &owner;
		OkBtn(PacketTemplateWindow &);
		void Do(TCommand &);
	} okBtn;
	class CancelBtn: public TEvent
	{
	public:
		PacketTemplateWindow &owner;
		CancelBtn(PacketTemplateWindow &);
		void Do(TCommand &);
	} cancelBtn;
private:
	GridWindow gridViewer;
public:
	GridNotify grid;
public:
	PacketTemplateData<Table, Col> data;
public:
	HWND hWnd, hOk, hCancel, hResult;
	wchar_t result[128];
	PacketTemplateWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TNotify &);

	void RClick(int, wchar_t *);
	void LClick(int, wchar_t *);
	static wchar_t *Title();
};

#pragma warning(disable : 4355)
template<class Table, class Col>PacketTemplateWindow<Table, Col>::PacketTemplateWindow()
	: okBtn(*this)
	, cancelBtn(*this)
{}  

template<class Table, class Col>void PacketTemplateWindow<Table, Col>::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;
	int y = l.Height - 50;
	grid.Size(0, 0, l.Width, y);

	int width = (l.Width - 30) / 2;

	MoveWindow(hOk, 10, y + 10, width, 30, false);
	MoveWindow(hCancel, 20 + width, y + 10, width, 30, false);
}

template<class Table, class Col>void PacketTemplateWindow<Table, Col>::operator()(TCommand &l)
{
	EventDo(l);
}	

namespace		
{															
	struct MainFile{};										
	MENU_TEXT(L"Файл", TopMenu<MainFile>)					

	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

	MENU_ITEM(L"Выход", MainExit)							   

		template<>struct TopMenu<MainFile>						   
	{														   
		typedef TL::MkTlst<									   
			MenuItem<MainExit>								   
		>::Result list;										   
	};														   

	template<class Table, class Col>struct Options{};										   
	template<class Table, class Col>struct NameMenu<TopMenu<Options<Table, Col>> >{wchar_t *operator()(HWND){return L"Настройки";}};

	template<class Table, class Col>struct NameMenu<MenuItem<Options<Table, Col>> >{wchar_t *operator()(HWND){return L"Сохранить координаты окна";}};
	template<class Table, class Col>struct Event<MenuItem<Options<Table, Col>> >:WindowPositionDlg<PacketTemplateWindow<Table, Col>>{};

	template<class Table, class Col>struct TopMenu<Options<Table, Col>>						   
	{														   
		typename typedef TL::MkTlst<									   
			MenuItem<Options<Table, Col>>						   
		>::Result list;										   
	};														   								   
}

template<class Table, class Col>unsigned PacketTemplateWindow<Table, Col>::operator()(TCreate &l)
{
	typedef TL::MkTlst<	
		TopMenu<MainFile>	
		, TopMenu<Options<Table, Col>>	
	>::Result MainMenu;	
	result[0] = '\0';
	hResult = NULL;
	Menu<MainMenu>().Init(l.hwnd);
	gridViewer.SetRClick(this, &PacketTemplateWindow::RClick);
	gridViewer.SetLClick(this, &PacketTemplateWindow::LClick);
	gridViewer.SetData(&data);
	grid.Create(l, &gridViewer);
	SetRow(grid.hWnd, data.Count());

	hOk = CreateWindow(L"button", L"Применить"
					, WS_VISIBLE | WS_CHILD | WS_TABSTOP
					,0, 0, 0, 0, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
					);
	SetWindowLongPtr(hOk, GWLP_USERDATA, (LONG)&okBtn);

	hCancel = CreateWindow(L"button", L"Отмена"
					, WS_VISIBLE | WS_CHILD | WS_TABSTOP
					,0, 0, 0, 0, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
					);
	SetWindowLongPtr(hCancel, GWLP_USERDATA, (LONG)&cancelBtn);

	return 0;
}

template<class Table, class Col>void PacketTemplateWindow<Table, Col>::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	delete this;	
}

template<class Table, class Col>unsigned PacketTemplateWindow<Table, Col>::operator()(TNotify &l)
{
	return GridNotify::OwnerNotify(l);
}

template<class Table, class Col>void PacketTemplateWindow<Table, Col>::LClick(int col, wchar_t *s)
{
	wcsncpy(result, s, dimention_of(result));
}

namespace
{
	template<class Table, class Col>struct AddItem{};
	template<class Table, class Col>struct DelItem{};

	template<class Table, class Col>struct TopMenu<AddItem<Table, Col>>			   
	{										   
		typedef NullType list;				   
	};										   
	template<class Table, class Col>struct NameMenu<TopMenu<AddItem<Table, Col>> >{wchar_t *operator()(HWND){return L"Добавить";}};

	template<class Table, class Col>struct TopMenu<DelItem<Table, Col>>			   
	{										   
		typedef NullType list;				   
	};										   
	template<class Table, class Col>struct NameMenu<TopMenu<DelItem<Table, Col>> >{wchar_t *operator()(HWND){return L"Удалить";}};

	template<class O, class P>struct __item__
	{
		void operator()(O *o, P *)
		{
			 o->value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(*o);
		}
	};

	struct __OkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &o, HWND h)
		{	
			if(!TL::find<typename Owner::list, __test__>()(&o.items, &h))return;
			TL::foreach<typename Owner::list, __item__>()(&o.items, &h);
			EndDialog(h, TRUE);
		}
	};

	template<class Table, class Col>struct Event<TopMenu<AddItem<Table, Col>> >	   
	{										   
		static void Do(HWND h)				   
		{									   			
			Table ot;
			if(TemplDialog<ParametersBase, Table, TL::MkTlst<__OkBtn, CancelBtn>::Result>(ot).Do(h, L"Добавить"))
			{
				PacketTemplateWindow<Table, Col> *w = (PacketTemplateWindow<Table, Col> *)GetWindowLongPtr(h, GWLP_USERDATA);
				w->data.Add(ot.items.get<Col>().value);
				SetRow(w->grid.hWnd, 1);
				UpdateRow(w->grid.hWnd);
			}
		}									   
	};		

	template<class Table, class Col>struct Event<TopMenu<DelItem<Table, Col>> >	   
	{										   
		static void Do(HWND h)				   
		{									   
			PacketTemplateWindow<Table, Col> *o = (PacketTemplateWindow<Table, Col> *)GetWindowLongPtr(h, GWLP_USERDATA);
			if(o->data.Count() > 1)
			{				
				o->data.Del(o->result);
				UpdateRow(o->grid.hWnd);
				o->result[0] = '\0';
			}
			else
			{
				MessageBox(h, L" Должна быть хотя бы одна запись", L"Предупреждение!!!", MB_ICONEXCLAMATION);
			}
		}									   
	};		
}

template<class Table, class Col>void PacketTemplateWindow<Table, Col>::RClick(int col, wchar_t *s)
{
	typedef TL::MkTlst<
		TopMenu<AddItem<Table, Col>>
		, Separator<0>
		, TopMenu<DelItem<Table, Col>>
	>::Result popup_items_list;
	wcsncpy(result, s, dimention_of(result));
	PopupMenu<popup_items_list>::Do(hWnd, hWnd);
}

template<class Table, class Col>wchar_t *PacketTemplateWindow<Table, Col>::Title()
{
	return L"Выбор";
}

template<class Table, class Col>PacketTemplateWindow<Table, Col>::OkBtn::OkBtn(PacketTemplateWindow &owner)
	: owner(owner)
{}

template<class Table, class Col>void PacketTemplateWindow<Table, Col>::OkBtn::Do(TCommand &l)
{
	if('\0' != owner.result[0])
	{
		SetWindowText(owner.hResult, owner.result); 
		DestroyWindow(l.hwnd);
		return;
	}
	MessageBox(l.hwnd, L"Необходимо выбрать значение", L"Предупреждение!!!", MB_ICONEXCLAMATION);
}

template<class Table, class Col>PacketTemplateWindow<Table, Col>::CancelBtn::CancelBtn(PacketTemplateWindow &owner)
	: owner(owner)
{}

template<class Table, class Col>void PacketTemplateWindow<Table, Col>::CancelBtn::Do(TCommand &l)
{
	DestroyWindow(l.hwnd);
}
