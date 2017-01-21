#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "DiffApp/App.h"

MIN_EQUAL_VALUE(Offset<0>, 0)
MIN_EQUAL_VALUE(Offset<1>, 0)
MIN_EQUAL_VALUE(Offset<2>, 0)
MIN_EQUAL_VALUE(Offset<3>, 0)
MIN_EQUAL_VALUE(Offset<4>, 0)
MIN_EQUAL_VALUE(Offset<5>, 0)
MIN_EQUAL_VALUE(Offset<6>, 0)
MIN_EQUAL_VALUE(Offset<7>, 0)

MAX_EQUAL_VALUE(Offset<0>, 10000)
MAX_EQUAL_VALUE(Offset<1>, 10000)
MAX_EQUAL_VALUE(Offset<2>, 10000)
MAX_EQUAL_VALUE(Offset<3>, 10000)
MAX_EQUAL_VALUE(Offset<4>, 10000)
MAX_EQUAL_VALUE(Offset<5>, 10000)
MAX_EQUAL_VALUE(Offset<6>, 10000)
MAX_EQUAL_VALUE(Offset<7>, 10000)

PARAM_TITLE(Offset<0>, L"Датчик 1")
PARAM_TITLE(Offset<1>, L"Датчик 2")
PARAM_TITLE(Offset<2>, L"Датчик 3")
PARAM_TITLE(Offset<3>, L"Датчик 4")
PARAM_TITLE(Offset<4>, L"Датчик 5")
PARAM_TITLE(Offset<5>, L"Датчик 6")
PARAM_TITLE(Offset<6>, L"Датчик 7")
PARAM_TITLE(Offset<7>, L"Датчик 8")

void OffsetsSensorsDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, OffsetsTable>(Singleton<OffsetsTable>::Instance()).Do(h, L"Смещения датчиков"))
	{
		App::InitCaretka();
	}
}