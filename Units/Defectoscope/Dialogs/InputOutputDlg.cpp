#include "stdafx.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "Dialogs.h"
#include "App/AppBase.h"
#include "DiffApp/App.h"
#include "1730/Device1730.h"
#include "InputOutputDlg.h"

namespace
{
template<class O, class P>struct __set__
{
	typedef typename TL::Inner<O>::Result T;
	void operator()(O *o, P *p)
	{
		wchar_t buf[128];
		GetWindowText(o->hWnd, buf, dimention_of(buf));
		o->value.value = p->Set<O>(Wchar_to<typename T::type_value>()(buf));
	}
};
template<class List>struct __data__
{
	typedef List list;
	unsigned buf[TL::Length<List>::value];
	template<class T>unsigned Set(unsigned val)
	{
		buf[TL::IndexOf<list, T>::value] = val;
		return 1 << val;
	}
};
template<class O, class P>struct __compress_bits__
{
	void operator()(O *o, P *p)
	{
		unsigned t = p->get<O>().value;
		unsigned i = 0;
		while(t >>= 1) ++i;
		o->value = i;
	}
};
struct putsDlg_OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(TestPassword<Owner::Base, Owner::Table>()(h))
		{
			if(!TL::find<Owner::list, __test__>()(&owner.items, &h))return;
			__data__<Owner::list> d;
			TL::foreach<Owner::list, __set__>()(&owner.items, &d);
			for(int i = 0; i < dimention_of(d.buf) - 1; ++i)
			{
				unsigned value = d.buf[i];
				for(int j = i + 1; j < dimention_of(d.buf); ++j)
				{
					if(value == d.buf[j])
					{
						MessageBox(h, L"Несколько данных ссылаются на один бит", L"Ошибка!!!", MB_ICONERROR);
						return;
					}
				}
			}
			CBase base(Owner::Base().name());
			if(base.IsOpen())
			{
				CopyFactory::Do(Singleton<Owner::Table>::Instance().items, owner.table.items);
				UpdateWhere<Owner::Table>(owner.table, base).ID(1).Execute();
			}
			EndDialog(h, TRUE);
		}
	}
};


struct DefaultBtn
{
	static const int width = 160;
	static const int height = 30;
	static const int ID = IDRETRY;
	wchar_t *Title(){return L"Встроенные настройки";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		if(TestPassword<Owner::Table>()(h))
		{
			CBase base(Owner::Base().name());
			if(base.IsOpen())
			{
				Owner::Table t;
				CopyFactory::Do(Singleton<Owner::Table>::Instance().items, t.items);
				UpdateWhere<Owner::Table>(t, base).ID(1).Execute();
			}
			EndDialog(h, TRUE);
		}
	}
};
//----------------------------------------------------------------------------
MIN_EQUAL_VALUE(iСontrolСircuits, 0)
MIN_EQUAL_VALUE(iCycle          , 0)
MIN_EQUAL_VALUE(iReady          , 0)
MIN_EQUAL_VALUE(iControl        , 0)
MIN_EQUAL_VALUE(iBase           , 0)
MIN_EQUAL_VALUE(iReserve        , 0)

MAX_EQUAL_VALUE(iСontrolСircuits, 31)
MAX_EQUAL_VALUE(iCycle          , 31)
MAX_EQUAL_VALUE(iReady          , 31)
MAX_EQUAL_VALUE(iControl        , 31)
MAX_EQUAL_VALUE(iBase           , 31)
MAX_EQUAL_VALUE(iReserve        , 31)


MIN_EQUAL_VALUE(oWork      , 0)
MIN_EQUAL_VALUE(oToShiftThe, 0)
MIN_EQUAL_VALUE(oResult1   , 0)
MIN_EQUAL_VALUE(oResult2   , 0)
MIN_EQUAL_VALUE(oPowerBM   , 0)
MIN_EQUAL_VALUE(oReserve   , 0)

MAX_EQUAL_VALUE(oWork      , 31)
MAX_EQUAL_VALUE(oToShiftThe, 31)
MAX_EQUAL_VALUE(oResult1   , 31)
MAX_EQUAL_VALUE(oResult2   , 31)
MAX_EQUAL_VALUE(oPowerBM   , 31)
MAX_EQUAL_VALUE(oReserve   , 31)

template<>struct DlgSubItems<iСontrolСircuits, unsigned >: UpDownSubItem<iСontrolСircuits>{};
template<>struct DlgSubItems<iCycle          , unsigned >: UpDownSubItem<iCycle          >{};
template<>struct DlgSubItems<iReady          , unsigned >: UpDownSubItem<iReady          >{};
template<>struct DlgSubItems<iControl        , unsigned >: UpDownSubItem<iControl        >{};
template<>struct DlgSubItems<iBase           , unsigned >: UpDownSubItem<iBase           >{};
template<>struct DlgSubItems<iReserve        , unsigned >: UpDownSubItem<iReserve        >{};

template<>struct DlgSubItems<oWork      , unsigned >: UpDownSubItem<oWork      >{};
template<>struct DlgSubItems<oToShiftThe, unsigned >: UpDownSubItem<oToShiftThe>{};
template<>struct DlgSubItems<oResult1   , unsigned >: UpDownSubItem<oResult1   >{};
template<>struct DlgSubItems<oResult2   , unsigned >: UpDownSubItem<oResult2   >{};
template<>struct DlgSubItems<oPowerBM   , unsigned >: UpDownSubItem<oPowerBM   >{};
template<>struct DlgSubItems<oReserve   , unsigned >: UpDownSubItem<oReserve   >{};

DO_NOT_CHECK(Descriptor1730)
PARAM_TITLE(Descriptor1730, L"Дескриптор платы 1730")
}
//---------------------------   ----------------------------   ---------------------------
void InputsDlg::Do(HWND h)
{
	InputBitTable t;
	TL::foreach<InputBitTable::items_list, __compress_bits__>()(&t.items, &Singleton<InputBitTable>::Instance().items);
	if(TemplDialog<ParametersBase, InputBitTable, TL::MkTlst<putsDlg_OkBtn, CancelBtn>::Result>(t).Do(h, L"Смещения бит входного порта"))
	{
	}
}
//-----------------------------------------------------------------------------
void OutputsDlg::Do(HWND h)
{
	OutputBitTable t;
	TL::foreach<OutputBitTable::items_list, __compress_bits__>()(&t.items, &Singleton<OutputBitTable>::Instance().items);
	if(TemplDialog<ParametersBase, OutputBitTable, TL::MkTlst<putsDlg_OkBtn, CancelBtn>::Result>(t).Do(h, L"Смещения бит выходного порта"))
	{
	}
}
//----------------------------------------------------------------------------
void Descriptor1730Dlg::Do(HWND h)
{
	Descriptor1730Table &t = Singleton<Descriptor1730Table>::Instance();
	if(TemplDialog<ParametersBase, Descriptor1730Table>(t).Do(h, L"Дескриптор платы 1730"))
	{
		device1730.Destroy();
		Sleep(500);
		if(!device1730.Init(t.items.get<Descriptor1730>().value))
		{
			MessageBox(0, L"Не могу инициировать плату 1730", L"Ошибка !!!", MB_ICONERROR);
		}
	}
}