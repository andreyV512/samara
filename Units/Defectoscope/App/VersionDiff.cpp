#include "stdafx.h"
#include "VersionDiff.h"
#include "App/AppBase.h"
#include "ItemsData/USPCData.h"
#include "debug_tools/DebugMess.h"
#include "DiffApp/App.h"


namespace Version
{
	static const int __magic_word__ = 0xcafe0000;
	static const int __version__ = 6;

	template<class O, class P>struct __store__
	{
		void operator()(P *p)
		{
			fwrite(&Singleton<O>::Instance(), sizeof(O), 1, p); 
		}
	};
#ifdef VIEWER_APP
	template<class O, class P>struct __load__
	{
		void operator()(P *p)
		{
			fread(&Singleton<O>::Instance(), sizeof(O), 1, p); 
		}
	};
#endif

#ifdef DEFECT_APP
	template<class O, class P>struct __load__
	{
		void operator()(P *p)
		{
			O o;
			fread(&o, sizeof(O), 1, p); 
		}
	};
//#pragma message ("после отладки удалить")
//	template<class P>struct __load__<ColorTable, P>
//	{
//		typedef ColorTable O;
//		void operator()(P *p)
//		{
//			O o;
//			fread(&o, sizeof(O) - sizeof(Clr<Cancel<Projectionist>>), 1, p); 
//		}
//	};
#endif


	typedef TL::MkTlst<Thickness, Long, Cross>::Result unit_list;
	struct __unit_data__
	{
		FILE *file;
		__unit_data__( FILE *file)
			: file(file)
		{}
	};
	template<class O, class P>struct __unit_store__
	{
		void operator()(P p)
		{
			ItemData<O> &d = Singleton<ItemData<O>>::Instance();
			fwrite(d.param, sizeof(d.param), 1, p.file);
		}
	};
#ifdef VIEWER_APP
	template<class O, class P>struct __unit_load__
	{
		void operator()(P p)
		{
			ItemData<O> &d = Singleton<ItemData<O>>::Instance();
			fread(d.param, sizeof(d.param), 1, p.file);
		}
	};
#endif
#ifdef DEFECT_APP
	template<class O, class P>struct __unit_load__
	{
		void operator()(P p)
		{
			ItemData<O>::TParam param[App::count_sensors]; 
			fread(param, sizeof(param), 1, p.file);
		}
	};
#endif

	template<class O, class P>struct __cancel_operator_store__
	{
		void operator()(P *p)
		{
			bool (&b)[App::count_sensors][App::count_zones] = Singleton<ItemData<O>>::Instance().cancelOperatorSensor;
			unsigned char c[App::count_zones] = {};
			for(int i = 0; i < App::count_zones; ++i)
			{
				for(int j = 0; j < App::count_sensors; ++j)
				{
					if(b[j][i]) c[i] |= 1 << j;
				}
			}
			fwrite(c, sizeof(c), 1, p);
		}
	};

	template<class O, class P>struct __cancel_operator_load__
	{
		void operator()(P *p)
		{
			bool (&b)[App::count_sensors][App::count_zones] = Singleton<ItemData<O>>::Instance().cancelOperatorSensor;
			unsigned char c[App::count_zones];
			fread(c, sizeof(c), 1, p);
			for(int i = 0; i < App::count_zones; ++i)
			{
				for(int j = 0; j < App::count_sensors; ++j)
				{
					b[j][i] = 0 != ((1 << j) & c[i]);
				}
			}			
		}
	};

	void SaveToFile(FILE *f)
	{
		unsigned t = __magic_word__ | __version__;
		fwrite(&t, sizeof(unsigned), 1, f);
		TL::foreach<ParametersBase::type_list, __store__>()(f);
		TL::foreach<unit_list, __unit_store__>()(__unit_data__(f));
		TL::foreach<unit_list, __cancel_operator_store__>()(f);
	}

	class USPCIniFile
	{
	public:
	   double scope_range    [8];
	   double scope_offset   [8];
	   double gateIF_position[8];
	   double gateIF_width   [8];
	   double gateIF_level   [8];
	   double gate1_width    [8];
	   double gate1_position [8];
	   double gate1_level    [8];
	};

	bool LoadFromFile(unsigned ver, FILE *f)
	{
		if(__magic_word__ == (ver & 0xffff0000))
		{
			int v = ver & 0xffff;
			switch(v)
			{
			case __version__:
				{
					TL::foreach<ParametersBase::type_list, __load__>()(f);
					TL::foreach<unit_list, __unit_load__>()(__unit_data__(f));
					TL::foreach<unit_list, __cancel_operator_load__>()(f);
					App::InitCaretka();
				}
				return true;
			case 5:
				{
					TL::foreach<unit_list, __unit_load__>()(__unit_data__(f));
				}
				return true;
			case 4:
				{
					double scope_velocity[App::count_sensors];
					fread(scope_velocity, sizeof(scope_velocity), 1, f);
					
					USPCIniFile u;// = Singleton<USPCIniFile>::Instance();
	            	fread(&u, sizeof(USPCIniFile), 1, f);
					dprint("ok");
				}
				return true;
			case 2:
				{
					double scope_velocity[App::count_sensors];
					fread(scope_velocity, sizeof(scope_velocity), 1, f);

					//wchar_t *s = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
					NameParam::type_value s;
					fread(s, sizeof(NameParam::type_value), 1, f);
				}
				return true;
			case 1:
				{
					//ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
					double x[8];
					fread(x, sizeof(x), 1, f);
				}
				return true;
			}
		}
		return false;
	}

	void RestoreMemory()
	{
		//ItemData<Thickness> &d = Singleton<ItemData<Thickness>>::Instance();
		//memcpy(d.scope_velocity, stored_items.value, sizeof(d.scope_velocity)); 
	}
	//void StoreMemory(){}
	void StoreScopeVelocity(int i, double data)
	{
		//stored_items.value[i] = data;
	}
}
