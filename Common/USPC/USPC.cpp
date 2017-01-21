#include "stdafx.h"
#include "USPC.h"
#include "ItemsData/USPCData.h"
#include "App/AppBase.h"
#include "USPC/ut_files.h"
#include "debug_tools/DebugMess.h"		
#include "App/Config.h"
#include "App/VersionDiff.h"
#ifndef DEBUG_ITEMS
#pragma comment(lib, "..\\..\\Common\\USPC\\USPC7100.lib")
#else
ULONG WINAPI USPC7100_Acq_Start
    (
    INT Board,                              // [in]
    INT NumberOfScansToAcquire              // [in]
	){return 1;}
ULONG WINAPI USPC7100_Open
    (
    INT boot                                // [in]
	){return 1;}
ULONG WINAPI USPC7100_Load
    (
    INT Board,                              // [in]
    INT Test,                               // [in]
    LPCSTR file                             // [in]
	){return 1;}
ULONG WINAPI USPC7100_Close(void){return 1;}
ULONG WINAPI USPC7100_Acq_Stop
    (
    INT Board                               // [in]
	){return 1;}
ULONG WINAPI USPC7100_Acq_Get_Status
    (
    INT Board,                              // [in]
    ULONG *Status,                          // [out]
    ULONG *NumberOfScansAcquired,           // [out]
    ULONG *NumberOfScansRead,               // [out]
    ULONG *BufferSize,                      // [out]
    ULONG *BlocSize                         // [out]
	){return 1;}
ULONG WINAPI USPC7100_Acq_Read
    (
    INT Board,                              // [in]
    INT NumberOfScansToRead,                // [in]
    INT TimeOut,                            // [in]
    ULONG *NumberRead,                      // [out]
    ULONG *ScansBacklog,                    // [out]
    UCHAR *pData                            // [out]
	){return 1;}
ULONG WINAPI USPC7100_Acq_Config(int Board, ULONG AcqMode
			, ULONG StartMode, ULONG AcqCondition[8], int PrePostScans
			, int FrequencyDivider, ULONG BufferSize, int * Unused, ULONG * SimplificationParam){return 1;}
ULONG WINAPI USPC7100_Read
    (
    INT Board,                              // [in]
    INT Test,                               // [in]
    INT Unit,                               // [in]
    LPCSTR strParam,                        // [in]
    double *dblValue,                       // [out]
    double *dblArrayValue1,                 // [out]
    double *dblArrayValue2,                 // [out]
    LPSTR StringValue                       // [out]
	){return 1;}
ULONG WINAPI USPC7100_Acq_Clear(int){return 1;}
#endif

namespace USPC
{
	wchar_t lastTypeSize[128];
	wchar_t (&currentTypeSize)[128] = Singleton<ParametersTable>::Instance().items.get<NameParam>().value.buffer;
	ItemData<Long> &longData = Singleton<ItemData<Long> >::Instance();
	ItemData<Cross> &clossData = Singleton<ItemData<Cross> >::Instance();
	ItemData<Thickness> &thickData = Singleton<ItemData<Thickness> >::Instance();

	bool open = false;

	template<class T>struct __board__;
	template<>struct __board__<Thickness>
	{
		static const int value = App::thickness_number;
	};
	template<>struct __board__<Long>
	{
		static const int value = App::long_number;
	};
	template<>struct __board__<Cross>
	{
		static const int value = App::cross_number;
	};

	template<class O, class P>struct __start__
	{
		bool operator()(P *p)
		{
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O>>().value)
			{
				int id = __board__<O>::value;
				unsigned err = USPC7100_Acq_Start(id, -1);
				dprint("USPC7100_Acq_Start %d  err %x\n", id, err);
				*p = err;
				return 0 == err;
			}
			return true;
		}
	};

	template<class O, class P>struct __clear__
	{
		bool operator()(P *p)
		{
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O>>().value)
			{
				unsigned err = USPC7100_Acq_Clear(__board__<O>::value);
				*p = err;
				return 0 == err;
			}
			return true;
		}
	};

	template<class O, class P>struct __config__
	{
		bool operator()(P *p)
		{
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O>>().value)
			{
				int id = __board__<O>::value;
				Singleton<ItemData<O> >::Instance().Start();
				ULONG conditions[8] = {};
				int fluidity = 64;
				unsigned err = USPC7100_Acq_Config(id, 0x1000, 1, conditions, 0, 1
					, 1024 * 64, &fluidity, NULL 
					);
				dprint("USPC7100_Acq_Config %d  err %x\n", id, err);
				*p = err;
				return 0 == err;
			}
			return true;
		}
	};

	struct __test_data__
	{
		ULONG status, err;
		int board;
		__test_data__(): status(0), err(0), board(-1){}
	};

	struct __uspc_read_data__
	{
		int unit;
		int sensor;
		int err;
	};

	template<class O, class P>struct __uspc_read__
	{
		bool operator()(O &o, P &p)
		{
			p.err = USPC7100_Read(p.unit, p.sensor, 0, (LPCSTR)o.Name(), &o.value, NULL, NULL, NULL);
			return !p.err;
		}
	};

	template<class P>struct __uspc_read__<gate1_TOF_WT_velocity, P>
	{
		typedef gate1_TOF_WT_velocity O;
		bool operator()(O &o, P &p)
		{
			p.err = USPC7100_Read(p.unit, p.sensor, 400, (LPCSTR)o.Name(), &o.value, NULL, NULL, NULL);
			return !p.err;
		}
	};

	template<class O>struct __read_param__
	{
		template<class P>int operator()(P *)
		{
			ItemData<O> &data = Singleton<ItemData<O> >::Instance();
			__uspc_read_data__ uspc_read_data;
			uspc_read_data.unit = __board__<O>::value;
			for(int i = 0; i < App::count_sensors; ++i)
			{
				uspc_read_data.sensor = i;
				if(!TL::find<ItemData<O>::param_list, __uspc_read__>()(data.param[i], uspc_read_data))
				{
					return uspc_read_data.err;
				}
			}
			return 0;
		}
	};

	template<class O, class P>struct __test__
	{
		bool operator()(P *p)
		{
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O>>().value)
			{
				int id = __board__<O>::value;
				Singleton<ItemData<O> >::Instance().Start();
				ULONG numberOfScansAcquired, numberOfScansRead, bufferSize, scanSize;
				Sleep(30);
				int err =  USPC7100_Acq_Get_Status(id, &p->status, &numberOfScansAcquired, &numberOfScansRead, &bufferSize, &scanSize);
				if(!err) err = __read_param__<O>()(p);
				p->board = id;
				p->err = err;
				return 0 == err;
			}
			return true;
		}
	};

	template<class O, class P>struct __stop__
	{
		void operator()()
		{
			Sleep(30);
			USPC7100_Acq_Stop(__board__<O>::value);
		}
	};

	template<class T, int MAX>struct __delay__
	{
		static unsigned Do()
		{
			static unsigned value;
			if(++value > MAX)
			{
				value = 0;
			}
			return value;
		}
	};

	template<class O, class P>struct __do__
	{
		bool operator()(P &p)
		{
			if(__delay__<O, 10>::Do()) return true;///< выполнятся будет примерно через каждые 50 мск
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O>>().value)
			{				
				ItemData<O> &item = Singleton<ItemData<O> >::Instance();
				ULONG numberRead;
				ULONG scansBacklog;
				UCHAR *pData = item.CurrentFrame();
				int id = __board__<O>::value;
				unsigned err = USPC7100_Acq_Read(
					id
					, -1
					, 0
					, &numberRead
					, &scansBacklog
					, pData
					);
				if(numberRead > 0)
				{
					item.OffsetCounter(numberRead);
				}
				p.err = err;
				p.sensor = id;
				return 0 == err;
			}
			return true;
		}
	};

	bool Open()
	{
		bool res = false;
		unsigned err = 0;
		wchar_t path[256];
		
		res = ExistCurrentUSPCFile(path);
		if(res)
		{
			err = USPC7100_Open(2);
			dprint("USPC7100_Open err %x\n", err);
			if(!err)
			{
				char b[1024];
				wcstombs(b, path, wcslen(path) + 1); 
				Sleep(100);
				err = USPC7100_Load(-1, -1, b);
				dprint("USPC7100_Load err %x\n", err);
			}
			res = 0 == err;
		}
		if(res)
		{
			__test_data__ data;
			res = TL::find<items_list, __test__>()(&data);
			if(!res)
			{
				dprint("test board %d  status %d err %d", data.board, data.status, data.err);
			}
		}
		return open = res;
	}

	void Close()
	{
		if(open)
		{
			unsigned err = USPC7100_Close();
			dprint("USPC7100_Close err %x\n", err);
			open = false;
		}
	}

	bool Config()
	{
		unsigned err = 0;
		bool b = TL::find<items_list, __config__>()(&err);
		dprint("config err %x\n", err);
		return b;
	}

	bool Start()
	{
		unsigned err = 0;
		bool b = TL::find<items_list, __start__>()(&err);
		if(!b)
		{
			TL::foreach<items_list, __stop__>()();
		}
		dprint("start err %x\n", err);
		return b;
	}

	void Stop()
	{
		unsigned err = 0;
		TL::foreach<items_list, __stop__>()();
		TL::find<items_list, __clear__>()(&err);
	}
struct __err__
{
	unsigned sensor;
	unsigned err;
};
	bool Do()
	{
		//int err = 0;
		__err__ err = {};
		bool b = TL::find<items_list, __do__>()(err);	
		if(err.err) dprint("USPC7100_Acq_Read err %x  sensor %d\n", err.err, err.sensor);
		return b;
	}

	struct __data_file__
	{
	   int unit;
	   int sensor;
	   wchar_t path[256];
	};

	template<class O, class P>struct __param_file__
	{
		bool operator()(O &o, P &p)
		{
			wchar_t dest[128];
			mbstowcs (dest, o.Name(), 128);
			return GetUsFileParam(o.value, p.unit, p.sensor, dest, p.path);
		}
	};

	DWORD WINAPI  __ConfigFromFile__(LPVOID)
	{
		__data_file__ data;
		bool res = ExistCurrentUSPCFile(data.path);
		
		if(res)
		{
			typedef Thickness Unit;
			ItemData<Unit> &unit = Singleton<ItemData<Unit> >::Instance();
			data.unit = __board__<Unit>::value;
			for(int i = 0; i < App::count_sensors; ++i)
			{
				data.sensor = i;
				res = TL::find<ItemData<Unit>::param_list, __param_file__>()(unit.param[i], data);
			}
		}

		if(res)
		{
			typedef Long Unit;
			ItemData<Unit> &unit = Singleton<ItemData<Unit> >::Instance();
			data.unit = __board__<Unit>::value;
			for(int i = 0; i < App::count_sensors; ++i)
			{
				data.sensor = i;
				res = TL::find<ItemData<Unit>::param_list, __param_file__>()(unit.param[i], data);
			}
		}

		if(res)
		{
			typedef Cross Unit;
			ItemData<Unit> &unit = Singleton<ItemData<Unit> >::Instance();
			data.unit = __board__<Unit>::value;
			for(int i = 0; i < App::count_sensors; ++i)
			{
				data.sensor = i;
				res = TL::find<ItemData<Unit>::param_list, __param_file__>()(unit.param[i], data);
			}
		}

		return 0;
	}

	void ConfigFromFile()
	{
		QueueUserWorkItem(__ConfigFromFile__, NULL, WT_EXECUTEDEFAULT);
	}
}