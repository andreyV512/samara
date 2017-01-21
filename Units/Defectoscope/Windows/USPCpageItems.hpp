#pragma once
#include "typelist.hpp"
#include "USPCParam.h"

template<class T>struct USCPpageItems
{
	typedef typename USPCParam::items_list params_list;

	typedef typename T::items_list items_list;
	
	int &unit;
	int &sens;

	TL::Factory<params_list> &params;

	template<class Z>USCPpageItems(Z &z)
		: unit(z.unit)
		, sens(z.sens)
		, params(z.uspcParam.items)
	{}
private:
	template<class List, class Z>struct __data__
	{
		typedef List list;
		typedef Z RET;
		RET *ret;
		USCPpageItems *self;
	};
	template<class O, class P>struct __sens__
	{
		bool operator()(O *o, P *p)
		{
			
			if(TL::IndexOf<P::list, O>::value == p->self->sens)
			{
				p->ret = &o->items.get<TL::Inner<P::RET>::Result>().items.get<P::RET>();
				return false;
			}
			return true;
		}
	};
	template<class O, class P>struct __unit__
	{
		bool operator()(O *o, P *p)
		{
			if(TL::IndexOf<P::list, O>::value == p->self->unit)
			{
				TL::find<typename O::items_list, __sens__>()(&o->items, (__data__<O::items_list, P::RET> *)p);
				return false;
			}
			return true;
		}
	};
public:	
	template<class Z>Z &get()
	{
		__data__<params_list, Z> data = {NULL, this};
		TL::find<params_list, __unit__>()(&params, &data);
		return *data.ret;
	}
};