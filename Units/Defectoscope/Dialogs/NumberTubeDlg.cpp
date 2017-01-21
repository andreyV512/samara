#include "stdafx.h"
#include "NumberTubeDlg.h"
#include "templates/typelist.hpp"
#include "Stored/StoredBase.h"
#include "DlgTemplates\ParamDlg.h"
#include "Stored/PacketBase.h"
#include "Windows/Common.h"
#include "Dialogs/Dialogs.h"
#include "DiffApp/App.h"
#include "Automat/Automat.h"
#include "Stored/ProtocolTable.h"
#include "Stored/StoredBase.h"

namespace
{
PARAM_TITLE(NumberTube, L"Номер трубы")
DO_NOT_CHECK(NumberTube)
struct ntCancelBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDCANCEL;
	wchar_t *Title(){return L"Закрыть";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
	    EndDialog(h, FALSE);
	}
};

struct ntOkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		wchar_t buf[512];
		DlgItem<NumberTube> &widget = owner.items.get<DlgItem<NumberTube>>();

		GetWindowText(widget.hWnd, buf, dimention_of(buf));
		if(wcslen(buf) > 30)
		{
			MessageBox(h, L"Длина номера трубы превышена", L"Предупреждение", MB_ICONEXCLAMATION);
			return;
		}
        if(0 == buf[0])
		{
			MessageBox(h, L"Необходимо ввести номер трубы", L"Предупреждение", MB_ICONEXCLAMATION);
			return;
		}

		if(TestNumberTableExist(buf))
		{
			int ret = MessageBox(h, L"Труба с текущим номером есть в пакете, продолжить?", L"Предупреждение"
				, MB_ICONEXCLAMATION | MB_YESNO
				);
			if(IDNO == ret)
			{
				return;
			}
		}
		widget.value.value = buf;
		EndDialog(h, TRUE);
	}
};

}

struct PacketXXXX
{
	typedef TL::MkTlst<		
		NumberTubeTable
	>::Result type_list;
	typedef TL::Factory<type_list> TTables;
	TTables tables;
	  wchar_t path[512];
	 const wchar_t *name();
};

bool NumberTubeDlg::Do()
{
	HWND h = App::MainWindowHWND();
	NumberTubeTable nt;
	nt.items.get<NumberTube>().value = Singleton<NumberTube>::Instance().value;
	bool b = TemplDialog<PacketXXXX, NumberTubeTable, TL::MkTlst<ntOkBtn, ntCancelBtn>::Result>(nt).Do(h, L"Ввод номера трубы");
	Singleton<NumberTube>::Instance().value = nt.items.get<NumberTube>().value;
	return b;
}
