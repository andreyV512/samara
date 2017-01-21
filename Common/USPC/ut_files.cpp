#include "stdafx.h"
#include "USPC/ut_files.h"
#include <Shlwapi.h>
#include <direct.h>
#include "DiffApp/App.h"
#include "App/AppBase.h"
#include "Registry.h"
#include "App/Config.h"

bool NewUSPCFile(HWND h, wchar_t *file)
{
	RegistryPathUTFile appPath;
	wchar_t src[256];
	wchar_t dest[256];

	wsprintf(src, L"%s\\default.us", appPath());
	wsprintf(dest, L"%s\\%s.us", appPath(), file);

	bool b = true;
	if(0 == CopyFile(src, dest, true))
	{
		b = false;
		int err = GetLastError();		
		wchar_t buffer[1024];
		if(2 == err)
		{
			wsprintf(buffer, L"Отсутствует файл %s", src);
			MessageBox(h, buffer, L"Ошибка!!!", MB_ICONHAND);
		}
		else if(80 == err)
		{
			wsprintf(buffer, L"Файл существует %s", dest);
			MessageBox(h, buffer, L"Предупреждение!!!", MB_ICONHAND);
			return true;
		}
		else if(FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, 
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buffer, 
			1024, 
			NULL))
		{
			MessageBox(h, buffer, L"Ошибка!!!", MB_ICONHAND);
		}		
	}
	return b;
}

bool ExistCurrentUSPCFile(wchar_t (&file)[256])
{
	wchar_t *s = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	RegistryPathUTFile appPath;
	wchar_t src[512];
	wsprintf(src, L"%s\\%s.us", appPath(), s);
//#ifndef DEBUG_ITEMS
	bool b = 0 != PathFileExists(src);
//#else
//	bool b = true;
//#endif
	if(b)
	{
		wcscpy(file, src);
	}
	else
	{
		wcscpy(file, L"Файл инициализации платы \"USPC7100\" отсутствует");
		wchar_t buf[1024];
		wsprintf(buf, L"Файл %s\nинициализации платы \"USPC7100\" отсутствует", src);
		MessageBox(App::MainWindowHWND(), buf, L"Ошибка!!!", MB_ICONERROR);
	}
	return b;
}

bool GetUsFileParam(double &value, int board, int sensor, wchar_t *name, wchar_t *path)
{
	wchar_t b[] = L"[Board 0]";
	b[dimention_of(b) - 3] = '0' + board;

	wchar_t s[] = L"[Test 0]";
	s[dimention_of(s) - 3] = '0' + sensor;

	wchar_t buf[256];
	FILE *f = _wfopen(path, L"rt");
	bool ok = false;
	wchar_t *res;
	if(f)
	{
		while(res = fgetws(buf, dimention_of(buf), f))
		{
			if(0 == wcsncmp(res, b, dimention_of(b) - 1))	{ok = true; break;}
		}

		if(ok)
		{
			ok = false;
			while(res = fgetws(buf, dimention_of(buf), f))
			{
				if(0 == wcsncmp(res, s, dimention_of(s) - 1))	{ok = true; break;}
			}
		}

		if(ok)
		{
			ok = false;
			int n = (int)wcslen(name);
			while(res = fgetws(buf, dimention_of(buf), f))
			{
				int k = 0;
				for(; res[k]; ++k) if('=' == res[k])break;
				if(n == k && 0 == wcsncmp(res, name, n))
				{
					res = &res[1 + wcslen(name)];
					value = _wtof(res);
					ok = true; 
					break;
				}
			}
		}

		fclose(f);
	}

	return ok;
}
