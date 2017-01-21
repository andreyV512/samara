#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "DiffApp/App.h"

MIN_EQUAL_VALUE(ReferenceOffset1, 1000)
MAX_EQUAL_VALUE(ReferenceOffset1, 10000)

PARAM_TITLE(ReferenceOffset1, L"Базовое расстояние(мм.)")

void BaseLengthDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, AdditionalSettingsTable>(Singleton<AdditionalSettingsTable>::Instance()).Do(h, L"Базовое расстояние"))
	{
	}
}