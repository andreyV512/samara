#include "stdafx.h"
#include "CrossThresholdWindow.h"
#include "ViewersMenu.hpp"

CrossThresholdWindow::CrossThresholdWindow()
	: minAxesY(Singleton<AxesTable>::Instance().items.get<AxesYMin<Cross> >().value)
	, maxAxesY(Singleton<AxesTable>::Instance().items.get<AxesYMax<Cross> >().value)
{}

TOP_MENU(Cross)
unsigned CrossThresholdWindow::operator()(TCreate &l)
{
	Menu<TopMenuCross::MainMenu>().Init(l.hwnd);
	(*(Parent *)this)(l);

	memmove(brak
		, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<Cross> >().value
		, sizeof(brak)
		);
	memmove(klass2
		, Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<Cross> >().value
		, sizeof(klass2)
		);
	return 0;
}

void CrossThresholdWindow::operator()(TSize &l)
{
	chart.minAxesY = minAxesY;
	chart.maxAxesY = maxAxesY;
	(*(TresholdWindow *)this)(l);
}

void CrossThresholdWindow::operator()(TClose &l)
{
	ThresholdsTable &table = Singleton<ThresholdsTable>::Instance();
	bool changedDefect = false;
	double *data = table.items.get<BorderDefect<Cross> >().value;
	for(int i = 0; i < dimention_of(brak); ++i)
	{
		if(data[i] != brak[i])
		{
			changedDefect = true;
			break;
		}
	}
	bool changedKlass2 = false;
	data = table.items.get<BorderKlass2<Cross> >().value;
	for(int i = 0; i < dimention_of(brak); ++i)
	{
		if(data[i] != klass2[i])
		{
			changedKlass2 = true;
			break;
		}
	}
	if(changedDefect || changedKlass2)
	{
		int ret = MessageBox(l.hwnd, L"Данные были изменены!\nСохранить?", L"Cообщение", MB_ICONQUESTION | MB_YESNOCANCEL);
		if(IDYES == ret)
		{
			if(TestPassword<ParametersBase, ThresholdsTable>()(l.hwnd))
			{
				if(changedDefect)
				{
					memmove(
						table.items.get<BorderDefect<Cross> >().value
						, brak
						, sizeof(brak)
						);
				}
				if(changedKlass2)
				{
					memmove(
						table.items.get<BorderKlass2<Cross> >().value
						, klass2
						, sizeof(klass2)
						);
				}
				CBase base(ParametersBase().name());
				if(base.IsOpen())
				{
					int id = CurrentId<ID<ThresholdsTable> >();

					if(1 == CountId<ID<ThresholdsTable> >(base, id))
					{
						Update<ThresholdsTable> update(base);
						if(changedDefect)  update.set<BorderDefect<Cross> >(brak);
						if(changedKlass2)  update.set<BorderKlass2<Cross> >(klass2);
						update.Where().ID(id).Execute();
					}
					else
					{
						Insert_Into<ThresholdsTable>(table, base).Execute();
						int id = Select<ThresholdsTable>(base).eq_all<ThresholdsTable::items_list>(&table.items).Execute();
						UpdateId<ID<ThresholdsTable> >(base, id);
					}
				}
			}
		}
		if(IDCANCEL == ret) return;
	}
	DestroyWindow(l.hwnd);
}
//------------------------------------------------------
wchar_t *CrossThresholdWindow::Title()
{
	return L"Поперечные пороги";
}
