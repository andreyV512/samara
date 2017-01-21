#include "stdafx.h"
#include "ItemsData/ConstData.h"
#include "App/AppBase.h"
#include "DiffApp/App.h"
#include "MessageText\SelectMessage.h"
#include "debug_tools/DebugMess.h"
namespace ConstData{
	
	unsigned *buf[TL::Length<ColorTable::items_list>::value];	

	template<class O, class P>struct __init__
	{
		void operator()(O *o)
		{
			static const int i = TL::IndexOf<ColorTable::items_list, O>::value;
			buf[i] = (unsigned *)&o->value;
		}
	};

	void Init()
	{
		TL::foreach<ColorTable::items_list, __init__>()(&Singleton<ColorTable>::Instance().items);
	}

	unsigned ZoneColor(unsigned c)
	{		
		if(c < dimention_of(buf))
		{
				return *buf[c];
		}
		return 0xBADC0105;
	}
}