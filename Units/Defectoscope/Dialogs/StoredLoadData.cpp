#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "window_tool/DlgFiles.h"
#include "Stored\Stored.h"
#include "Compute/Compute.h"
#include "window_tool\AnimationControl.h"
#include "window_tool/Zip.h"
#pragma warning(disable: 4996)
namespace
{
	wchar_t path[512];
	struct __load__
	{		
		__load__(wchar_t *path_)
		{
			wcscpy(path, path_);
			QueueUserWorkItem(ThreadProc, NULL, WT_EXECUTEDEFAULT);
		}
		static DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
		{
			AnimationWindow::Prepare();

			int offs = (int)wcslen(path) - 4;
			bool deleteFile = false;
			if(0 == wcscmp(&path[offs], L".bz2"))
			{
				Zip::UnZipFile2(path);
				path[offs] = 0;
				deleteFile = true;
			}

			Stored::DataFromFile(path);		
			if(deleteFile) DeleteFile(path);
			compute.Recalculation();			
			AnimationWindow::Destroy();
			return 0;
		}
	};
	struct __store__
	{		
		__store__(wchar_t *path_)
		{
			wcscpy(path, path_);
			QueueUserWorkItem(ThreadProc, NULL, WT_EXECUTEDEFAULT);
		}
		static DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
		{
			AnimationWindow::Prepare();
			int offs = (int)wcslen(path) - 4;
			bool archive = 0 == wcscmp(&path[offs], L".bz2");
			if(archive) path[offs] = '\0';
			Stored::DataToFile(path);
			if(archive)
			{
				Zip::ZipFile2(path);
			}
			AnimationWindow::Destroy();
			return 0;
		}
	};
}

void StoredDataDlg::Do(HWND h)
{
	SaveData o(h, L"Сохранить");
	if(o())
	{
		AnimationWindow::Init(h, L"Сохранение");
		__store__(o.sFile);
		return;
	}
	MessageBox(h, L"Файл не сохранён", L"Ошибка!!!", MB_ICONERROR);
}

void LoadDataDlg::Do(HWND h)
{
	OpenData o(h, L"Загрузить");
	if(o())
	{
		AnimationWindow::Init(h, L"Загрузка");
		
		__load__(o.sFile); 
		wchar_t *s = o.sFile;
		int len = (int)wcslen(s);
		int i = len - 1;
		for(; i > 0; --i)
		{
			if('.' == s[i])
			{
				s[i] = 0;
				break;
			}
		}
		for(; i > 0; --i)
		{
			if('/' == s[i] || '\\' == s[i])
			{
				break;
			}
		}
        wcscpy(app.nameFile, &s[i + 1]);
		return;
	}
	MessageBox(h, L"Файл не загружен", L"Ошибка!!!", MB_ICONERROR);
}