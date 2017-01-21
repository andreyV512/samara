#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "DiffApp/App.h"

DO_NOT_CHECK(N0)
PARAM_TITLE(N0, L"Первая граница")

DO_NOT_CHECK(N1)
PARAM_TITLE(N1, L"Вторая граница")

DO_NOT_CHECK(N2)
PARAM_TITLE(N2, L"Третья граница")
							
DO_NOT_CHECK(N3)
PARAM_TITLE(N3, L"Четвёртая граница")

void ProtectiveThickeningDlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, ProtectiveThickeningTable>(Singleton<ProtectiveThickeningTable>::Instance())
		.Do(h, L"Границы тела трубы"))
	{
	}
}