#include "stdafx.h"
#include "PacketWindow.h"
#include "templates/typelist.hpp"
#include "Stored/StoredBase.h"
#include "DlgTemplates\ParamDlg.h"
#include "Stored/PacketBase.h"
#include "Windows/Common.h"
#include "PacketTemplateWindow.hpp"

struct PacketWindow
{
	typedef TL::MkTlst<
		NumberPacket
		, NormativeDocument
		, Alloy
		, DeliveryStatus
		, ProductCodeNumber
		, Gang
		, Standart
		, Operator
	>::Result items_list;
	static const int ID_OK = TL::Length<items_list>::value + 2;
	static const int ID_CANCEL = 1 + ID_OK;

	HWND hOk, hCancel;
	HWND hItems[TL::Length<items_list>::value];
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam);
	bool Do(HWND );
	bool OkBtn();
};

namespace
{
	struct __push_button_data__
	{
		PacketWindow *e;
		WPARAM id;
		HWND h;
	};
	
	template<class O, class P>struct __push_button__
	{
		bool operator()(P *p)
		{
			return TL::IndexOf<PacketWindow::items_list, O>::value != p->id;
		}
	};

	template<class O, class P>struct __on_push_button__
	{
		bool operator()(P *p)
		{
			if(TL::IndexOf<PacketWindow::items_list, O>::value == p->id)
			{
				typedef PacketTemplateWindow<typename Table<O>::Result, O> Window;
				HWND hh = Common::OpenWindowBackGround<Window>::Do(p->h);
				Window *x = (Window *)GetWindowLongPtr(hh, GWLP_USERDATA);
				x->hResult = p->e->hItems[p->id];
				SetWindowPos(hh, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				LONG_PTR dwStyle = GetWindowLongPtr(hh, GWL_STYLE);
				dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
				SetWindowLongPtr(hh, GWL_STYLE, dwStyle);
				return false;
			}
			return true;
		}
	};

	struct __init_item_data__
	{
		PacketWindow *e;
		HWND h;
		int height;
		int edit_width;
		int label_width;
		int dy;
		__init_item_data__(PacketWindow *e, HWND h)
			: e(e)
			, h(h)
			, height(-20)
			, dy(30)
			, edit_width(320)
			, label_width(200)
		{}
	};
	template<class O, class P>struct __init_button_item__
	{
		void operator()(P *p)
		{
			static const int id = TL::IndexOf<PacketWindow::items_list, O>::value;
			p->e->hItems[id] = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit"
				, Singleton<O>::Instance().value
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_READONLY
				, 10, p->height += p->dy, p->edit_width, 25, p->h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			CreateWindow(L"button", L"..."
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP 
				,p->edit_width + 20, p->height, 25, 25, p->h, (HMENU)id, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			CreateWindow(L"static", ParamTitle<O>()()
				, WS_VISIBLE | WS_CHILD
				, p->edit_width + 50, p->height + 3, p->label_width, 20, p->h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
		}
	};
	template<class O, class P>struct __init_item__
	{
		void operator()(P *p)
		{
			static const int id = TL::IndexOf<PacketWindow::items_list, O>::value;
			p->e->hItems[id] = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit"
				, Singleton<O>::Instance().value
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
				, 10, p->height += p->dy, p->edit_width, 25, p->h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			
			CreateWindow(L"static", ParamTitle<O>()()
				, WS_VISIBLE | WS_CHILD
				, p->edit_width + 50, p->height + 3, p->label_width, 20, p->h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
		}
	};

	template<class T>struct Table;
#define TABLE(n)template<>struct Table<n>{typedef n##sTable Result;};

	#define __TITLE__(type, txt)\
	PARAM_TITLE(type, txt)\
	CHECK_EMPTY_STRING(type)\
	HEADER_TABLE(type, 600, txt)\
	template<int N>struct DlgSubItems<type, Holder<N> >: EditItems<type, 350>{};\
	template<class P>struct __push_button__<type, P>: __on_push_button__<type, P>{};\
	TABLE(type)\
	template<class P>struct __init_item__<type, P>: __init_button_item__<type, P>{};
///<Для отображения параметра вставить в items_list (смотри выше)
///<Данные с возможностью выбора значения из списка
	__TITLE__(NormativeDocument, L"Нормативный документ")
    __TITLE__(Operator, L"Оператор")
	__TITLE__(Alloy, L"Сплав")
	__TITLE__(DeliveryStatus, L"Состояние поставки")
	__TITLE__(Gang, L"Смена")
///<Данные без возможности выбора из списка
	PARAM_TITLE(NumberPacket, L"Партия")
	PARAM_TITLE(ProductCodeNumber, L"Шифр изделия")
	PARAM_TITLE(Standart, L"№СОП")
#undef __TITLE__
#undef TABLE
}

bool PacketWindowDlg(HWND h)
{
	PacketBase::Restore();
	return PacketWindow().Do(h);
};

LRESULT CALLBACK PacketWindow::Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		{
			PacketWindow *e = (PacketWindow *)GetWindowLongPtr(h, GWLP_USERDATA);	
			switch(wParam)
			{
			case ID_OK:
				if(e->OkBtn())EndDialog(h, TRUE);
				return TRUE;
			case ID_CANCEL:
				EndDialog(h, FALSE);
				return TRUE;
			default:
				{
					__push_button_data__ data = {e, wParam};
					if(!TL::find<items_list, __push_button__>()(&data)) return TRUE;
				}
			}
		}
		break;
	case WM_INITDIALOG:
		{			
			SetWindowLongPtr(h, GWLP_USERDATA, lParam);
			PacketWindow *e = (PacketWindow *)lParam;
			
			__init_item_data__ d(e, h);
			TL::foreach<items_list, __init_item__>()(&d);

			static const int width = d.edit_width + d.label_width + 30;	
			d.height += 40;
			RECT r;
			GetWindowRect(GetDesktopWindow(), &r);
			int btnWidth = width / 2 - 20;

			CreateWindow(L"button", L"Продолжить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP 
				, 10, d.height, btnWidth, 30, h, (HMENU)ID_OK, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			SendMessage(h, DM_SETDEFID, ID_OK, 0);

			CreateWindow(L"button", L"Отмена"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON
				, 20 + btnWidth, d.height, btnWidth, 30, h, (HMENU)ID_CANCEL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);

			d.height += 65;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - d.height) / 2;
			MoveWindow(h, x, y, width, d.height, true);			
		}
		return TRUE;
	}
	return FALSE;
}

bool PacketWindow::Do(HWND h)
{
	return TemplDlg_Do(h, L"Партия", (DLGPROC)Proc, (LPARAM)this);
}
namespace
{
	template<class O, class P>struct __test_text__
	{
		bool operator()(P *p)
		{
			wchar_t buf[128];
			HWND h = p[TL::IndexOf<PacketWindow::items_list, O>::value];
			GetWindowText(h, buf, 128);
			if(0 == buf[0])
			{
				wsprintf(buf, L"Парраметр \"%s\" не заполнен", ParamTitle<O>()());
				MessageBox(GetParent(h), buf, L"Ошибка!!!", MB_ICONHAND);
				return false;
			}
			return true;
		}
	};
	template<class X, class P>struct __test_text__<ID<X>, P>
	{
		bool operator()(P *p)
		{
			return true;
		}
	};
	template<class O, class P>struct __set_text__
	{
		void operator()(P *p)
		{
			wchar_t buf[128];
			GetWindowText(p[TL::IndexOf<PacketWindow::items_list, O>::value], buf, 128);
			Singleton<O>::Instance().value = buf;
		}
	};
}
bool PacketWindow::OkBtn()
{
	if(TL::find<items_list, __test_text__>()(hItems))
	{
		TL::foreach<items_list, __set_text__>()(hItems);
		PacketBase::Save();
		return true;
	}
	return false;
}