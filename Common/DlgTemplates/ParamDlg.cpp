#include "stdafx.h"
#include "DlgTemplates\ParamDlg.h"
#include "debug_tools/DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif
//-----------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
//--------------------------------------------------------------------------------
bool TemplDlg_Do(HWND hWnd, wchar_t *title, DLGPROC proc, LPARAM param)
{
	char *p = (char *)LocalAlloc(LPTR, 2048);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
    wcscpy(c, title);
	
	bool result = 0 != DialogBoxIndirectParam((HINSTANCE)::GetModuleHandle(NULL), &d, hWnd, proc, param);
    LocalFree((HLOCAL)p);
	return result;
}


