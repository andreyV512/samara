#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/AppBase.h"
#include "DiffApp/App.h"

namespace
{
DO_NOT_CHECK( BrakStrobe2<Thickness>)
PARAM_TITLE( BrakStrobe2<Thickness>, L"Расслоение(mm)")
}

void BrackStrobe2Dlg::Do(HWND h)
{
	if(TemplDialog<ParametersBase, BrackStrobe2Table>(Singleton<BrackStrobe2Table>::Instance()).Do(h, L"Расслоение"))
	{
	}
}