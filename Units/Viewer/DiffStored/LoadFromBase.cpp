#include "stdafx.h"
#include "LoadFromBase.h"
#include <ATLComTime.h>
#include "Stored/StoredBase.h"
#include "Base/ExpressBase.hpp"
#include "window_tool\AnimationControl.h"
#include "Stored\Stored.h"
#include "Compute/Compute.h"
#include "debug_tools/DebugMess.h"
#include "window_tool\Zip.h"

namespace FromBase
{
	int ToInt(wchar_t *d)
	{
		wchar_t buf[] = {d[0], d[1], 0};
		return _wtoi(buf);
	}
	template<class O, class P>struct __set_treshold__
	{
		void operator()(O &, P &){}
	};
	template<template<class>class W, class X, class P>struct __set_treshold__<ID<W<X>>, P>
	{
		void operator()(ID<W<X>> &o, P &base)
		{
			StoredThresholdsTable tt;
			Select<StoredThresholdsTable>(base).ID(o.value).Execute<StoredThresholdsTable>(tt);
			memmove(&Singleton<ThresholdsTable>::Instance().items.get<W<X>>().value, &tt.items.get<Treshold>().value, sizeof(typename W<X>::type_value));
		}
	};

	template<class O, class P>struct __set_treshold_XXXX__
	{
		void operator()(O &o, P &p)
		{
			Singleton<DeadAreaTable>::Instance().items.get<O>().value = o.value;
		}
	};

	template<class X, class P>struct __set_treshold__<DeadAreaMM0<X>, P>: __set_treshold_XXXX__<DeadAreaMM0<X>, P>{};
	template<class X, class P>struct __set_treshold__<DeadAreaMM1<X>, P>: __set_treshold_XXXX__<DeadAreaMM1<X>, P>{};

	template<class T, class D>struct __get_tresholds__
	{
		bool operator()(int, T &table, D &base)
		{
			TL::foreach<typename T::items_list, __set_treshold__>()(table.items, base);
			return true;
		}
	};	
	wchar_t __load_path[512];
	struct __load__
	{			
		__load__(wchar_t *path_)
		{
			wcscpy(__load_path, path_);
			QueueUserWorkItem(ThreadProc, NULL, WT_EXECUTEDEFAULT);
		}
		static DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
		{
			AnimationWindow::Prepare();
			int len = wcslen(__load_path);
            wcscat(__load_path, L".bz2");
			bool existZipFile = Zip::UnZipFile2(__load_path);
			__load_path[len] = '\0';
			Stored::DataFromFile(__load_path);		
			if(existZipFile)DeleteFile(__load_path);
	//		compute.CancelOperatorClear();
			compute.Recalculation();
			AnimationWindow::Destroy();
			return 0;
		}
	};
	void Load(HWND h, wchar_t *path)
	{
		if(0 == path[0]) return;
		int year  = ToInt(&path[0]) + 2000;
		int month = ToInt(&path[2]);
		int day   = ToInt(&path[4]);
		int hour  = ToInt(&path[6]);
		int min   = ToInt(&path[8]);
		int sec   = ToInt(&path[10]);
		COleDateTime current(year, month, day, hour, min, sec);

		bool b = COleDateTime::valid == current.GetStatus();

		if(b)
		{
			StoredBase parameters;

			CExpressBase base(
				parameters.name()
				, CreateDataBase<StoredBase::type_list, NullType, MSsql>()
				, parameters.tables
				);


			TubesTable tt;
			b = base.IsOpen();
			if(b)
			{
				dprint("base open ok\n");
				b = 0 != Select<TubesTable>(base).eq<Date_Time>(current).Execute(tt);

				if(b)
				{
					dprint("select current ok\n");
					b = Select<StoredMeshureTable>(base)
						.ID(tt.items.get<ID<StoredMeshureTable>>().value).ExecuteLoop<__get_tresholds__>(base);
					
					if(b) dprint("thresholds load ok ??? ");
				}
				base.Close();
			}
		}

		if(b)
		{
			dprint("load data  from base ok\n");
			wchar_t buf[1024];
			//wchar_t sub[1024];
#if 1
			DWORD length = GetModuleFileName( NULL, buf, 1024);
			PathRemoveFileSpec(buf);

			//wcscpy(sub, path);
			for(int i = wcslen(buf) - 1; i > 0; --i)
			{
				if('\\' == buf[i] || '/' == buf[i]) {buf[i] = 0; break;}
			}

			wsprintf(&buf[wcslen(buf)], L"\\Stored\\%s.dat", path);
#else
			wsprintf(buf, L"Stored\\%s.dat", path);
#endif
			dprint("%S\n", path);
			AnimationWindow::Init(h, L"Загрузка");
			__load__ load(buf);
		}
		else
		{
			MessageBox(h, L"Файл не загружен", L"Ошибка!!!", MB_ICONERROR);
		}
	}
}