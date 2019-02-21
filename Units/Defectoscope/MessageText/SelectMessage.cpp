#include "StdAfx.h"
#include "SelectMessage.h"
#include "App/AppBase.h"
#include "MessageText\LabelMessage.hpp"
#include "debug_tools/DebugMess.h"
#include "App/AppBase.h"
#include "templates\sort.hpp"

namespace SelectMesageN
{
	template<class >struct __selectItem__
	{
		bool value;
		__selectItem__() : value(false){}
	};
	template<>struct __selectItem__<Undefined>
	{
		bool value;
		__selectItem__() : value(true){}
	};
	typedef	TL::MkTlst<
		/*0*/	Undefined   
		/*1*/	, DeathZone
		/*2*/	, Nominal	
		/*3*/	, BorderAbove<Thickness> 
		/*4*/	, BorderLower<Thickness> 
		/*5*/	, BorderKlass2<Long>  
		/*6*/	, BorderDefect<Long> 
		/*7*/	, BorderKlass2<Cross>
		/*8*/	, BorderDefect<Cross>
		/*9*/   , BrakStrobe2<Thickness>
		/*10*/, Cancel<Projectionist>
	>::Result  __selected_list__;

	template<class O, class P>struct __set__
	{
		void operator()(P &p)
		{
			p.get<__selectItem__<O>>().value = true;
		}
	};

	template<class O, class P>struct __skip__
	{
		void operator()(P &p)
		{
			if(p.get<__selectItem__<O>>().value)
			{
				p.get<__selectItem__<Undefined>>().value = false;
				p.get<__selectItem__<Nominal>>().value = false;
			}
		}
	};

	template<class O, class P>struct __skip__X
	{
		void operator()(P &p)
		{
			if(p & (1 << TL::IndexOf<__selected_list__, O>::value))
			{
				p &= ~(1 << TL::IndexOf<__selected_list__, Undefined>::value);
				p &= ~(1 << TL::IndexOf<__selected_list__, Nominal>::value);
			}
		}
	};

	template<class P>struct __skip__X<Cancel<Projectionist>, P>
	{
		typedef Cancel<Projectionist> O;
		void operator()(P &p)
		{
			if(p & (1 << TL::IndexOf<__selected_list__, O>::value))
			{
				p &= ~(1 << TL::IndexOf<__selected_list__, Undefined>::value);
			}
		}
	};

	template<class P>struct __skip__<Nominal, P>
	{
		void operator()(P &p)
		{
			if(p.get<__selectItem__<Nominal>>().value)
			{
				p.get<__selectItem__<Undefined>>().value = false;
			}
		}
	};

	template<class P>struct __skip__X<Nominal, P>
	{
		void operator()(P &p)
		{
			if(p & (1 << TL::IndexOf<__selected_list__, Nominal>::value))
			{
				p &= ~(1 << TL::IndexOf<__selected_list__, Undefined>::value);
			}
		}
	};

	template<class P>struct __skip__X<DeathZone, P>
	{
		void operator()(P &p)
		{
			if(p & (1 << TL::IndexOf<__selected_list__, DeathZone>::value))
			{
				p = 1 << TL::IndexOf<__selected_list__, DeathZone>::value;
			}
		}
	};

	template<class P>struct __skip__<Undefined, P>
	{
		void operator()(P &p)
		{
		}
	};

	template<class P>struct __skip__X<Undefined, P>
	{
		void operator()(P &p)
		{
		}
	};

	template<class X, class P>struct __skip__<BorderDefect<X>, P>
	{
		void operator()(P &p)
		{
			if(p.get<__selectItem__<BorderDefect<X>>>().value)
			{
				p.get<__selectItem__<Undefined>>().value = false;
				p.get<__selectItem__<Nominal>>().value = false;
				p.get<__selectItem__<BorderKlass2<X>>>().value = false;
			}
		}
	};

	template<class X, class P>struct __skip__X<BorderDefect<X>, P>
	{
		void operator()(P &p)
		{
			if(p & (1 << TL::IndexOf<__selected_list__, BorderDefect<X>>::value))
			{
				p &= ~(1 << TL::IndexOf<__selected_list__, Undefined>::value);
				p &= ~(1 << TL::IndexOf<__selected_list__, Nominal>::value);
				p &= ~(1 << TL::IndexOf<__selected_list__, BorderKlass2<X>>::value);
			}
		}
	};

	template<class O, class P>struct __select__
	{
		bool operator()(P &p)
		{
			if(TL::IndexOf<label_message_list, O>::value == p.id)
			{
				TL::foreach<typename O::items_list, __set__>()(p.items);
				return false;
			}
			return true;
		}
	};

	template<class List, class T>struct __in_list__;

	template<class Head, class Tail, class T>struct __in_list__<Tlst<Head, Tail>, T>
	{
		static const bool value = __in_list__<Tail, T>::value;
	};

	template<class Tail, class T>struct __in_list__<Tlst<T, Tail>, T>
	{
		static const bool value = true;
	};

	template<class T>struct __in_list__<NullType, T>
	{
		static const bool value = false;
	};

	template<class List, class T>struct __filtre__;

	template<class Head, class Tail, class T>struct __filtre__<Tlst<Head, Tail>, T>
	{
		typedef typename TL::_if<__in_list__<typename Head::items_list, T>::value
			, Tlst<Head, typename __filtre__<Tail, T>::Result>
			, typename __filtre__<Tail, T>::Result
		>::Result Result;
	};

	template<class T>struct __filtre__<NullType, T>
	{
		typedef NullType Result;
	};

	template<class List, class T>struct __filtre_off__;

	template<class Head, class Tail, class T>struct __filtre_off__<Tlst<Head, Tail>, T>
	{
		typedef typename TL::_if<!__in_list__<typename Head::items_list, T>::value
			, Tlst<Head, typename __filtre_off__<Tail, T>::Result>
			, typename __filtre_off__<Tail, T>::Result
		>::Result Result;
	};

	template<class T>struct __filtre_off__<NullType, T>
	{
		typedef NullType Result;
	};

	template<class T>struct __data__
	{
		typedef T items_list;
		int &res;
		__data__(int &res): res(res){}
	};

	template<class List>struct __get_id__;
	template<class Head, class Tail>struct __get_id__<Tlst<Head, Tail>>
	{
		template<class O, class P>void operator()(O &o, P &p)
		{
			if(o.get<__selectItem__<Head>>().value)
			{
				typedef __filtre__<typename P::items_list, Head>::Result list;
				__get_id__<Tail>()(o, (__data__<list> &)p);
			}
			else
			{
				typedef __filtre_off__<typename P::items_list, Head>::Result list;
				__get_id__<Tail>()(o, (__data__<list> &)p);
			}
		}
	};

	template<class List>struct __sel_first__;
	template<class Head, class Tail>struct __sel_first__<Tlst<Head, Tail>>
	{
		static const int value = TL::IndexOf<label_message_list, Head>::value;
	};
	template<>struct __sel_first__<NullType>
	{
		static const int value = -1;
	};

	template<>struct __get_id__<NullType>
	{
		template<class O, class P>void operator()(O &o, P &p)
		{
			p.res = __sel_first__<typename P::items_list>::value;
		}
	};

	struct __result_data__
	{
		int id;
		typedef TL::ListToWapperList<__selected_list__, __selectItem__>::Result items_list;
		TL::Factory<items_list> items;
	};

	unsigned __bits__[TL::Length<label_message_list>::value];

	struct TSortedBit
	{
		unsigned id;
		unsigned __bits__;
		bool operator >(TSortedBit val){return __bits__ > val.__bits__;}
	};
	TSortedBit __sorted_bit__[TL::Length<label_message_list>::value];

	template<class T>struct shift_bits;
	template<class Head, class Tail>struct shift_bits<Tlst<Head, Tail>>
	{
		static const unsigned value = (1 << TL::IndexOf<__selected_list__, Head>::value) | shift_bits<Tail>::value;
	};
	template<>struct shift_bits<NullType>{static const unsigned value = 0;};

	template<class T>struct ClrLst;
	template<class _0, class _1, class _2, class _3, class _4, class _5>struct ClrLst<Clr<_0, _1, _2, _3, _4, _5> >
	{
		typedef typename TL::MkTlst<_0, _1, _2, _3, _4, _5>::Result Result;
	};

	template<class T, int i = 0>struct __set_bits__;
	template<class Head, class Tail, int i>struct __set_bits__<Tlst<Head, Tail>, i>: __set_bits__<Tail, 1 + i>
	{
		__set_bits__()
		{
			__bits__[i] = shift_bits<typename ClrLst<Head>::Result>::value;
			__sorted_bit__[i].__bits__ = __bits__[i];
			__sorted_bit__[i].id = i;
		}
	};
	template<int i>struct __set_bits__<NullType, i>{};

    __set_bits__<label_message_list> __init_set_bits__;
	
	struct Sort
	{
		Sort()
		{
			QuickSort(__sorted_bit__, dimention_of(__sorted_bit__));
		}
		unsigned operator()(unsigned res)
		{
			unsigned mid = (dimention_of(__sorted_bit__) - 1) / 2;
			unsigned start = 0;
			unsigned stop = dimention_of(__sorted_bit__) - 1;
			TSortedBit *xxx = __sorted_bit__; 
			while(stop != start)
			{
				if(__sorted_bit__[mid].__bits__ > res)
				{
					stop = mid;
					mid = (stop + start) / 2;
				}
				else if(__sorted_bit__[mid].__bits__ < res)
				{
					start = mid;
					mid = (stop + start) / 2;
				}				
				if(__sorted_bit__[mid].__bits__ == res)
				{
					break;
				}
				if(stop - start <= 1)
				{
					bool b = false;
					if(b =(__sorted_bit__[stop].__bits__ == res)) mid = stop;
					else if(b = (__sorted_bit__[start].__bits__ == res)) mid = start;
					if(!b)dprint("sort %d\n", res);
					break;
				}
			}
			return __sorted_bit__[mid].id;
		}
	};

	Sort sort;

	int Result(int *x)
	{
		
        unsigned res = 0;
		while(-1 != *x)
		{
			if(*x < dimention_of(__bits__)) res |= __bits__[*x];
			x = &x[1];
		}
		TL::foreach<__selected_list__, __skip__X>()(res);
	
		return sort(res);
	}
}

char *SelectMessage(int *x, int &res)
{
	res = SelectMesageN::Result(x);
	return NULL;
}

bool MinAndLamination(char status)
{
	static const unsigned mask = (1 << TL::IndexOf<SelectMesageN::__selected_list__, BorderLower<Thickness>>::value) 
		| (1 << TL::IndexOf<SelectMesageN::__selected_list__, BrakStrobe2<Thickness>>::value);
	return mask == (mask & SelectMesageN::__bits__[status]);
}

template<class T>struct __first__;
	template<class A, class B, class C, class D, class E,class F>struct __first__<Clr<A, B, C, D, E,F>>
	{
		typedef A Result;
	};

unsigned *__color__[TL::Length<label_message_list>::value];
template<class T, int i = 0>struct __set_color__;
template<class Head, class Tail, int i>struct __set_color__<Tlst<Head, Tail>, i>: __set_color__<Tail, 1 + i>
{
	__set_color__(){__color__[i] = (unsigned *)&Singleton<ColorTable>::Instance().items.get<Clr<typename __first__<Head>::Result> >().value;}
};
template<int i>struct __set_color__<NullType, i>{};

__set_color__<label_message_list> __init_set_color__;

void ColorBar::operator()(double &data, unsigned &color, int id, double defData)
{
   color = *__color__[id];
   if(TL::IndexOf<label_message_list, Clr<Undefined>>::value == id 
	   || TL::IndexOf<label_message_list, Clr<DeathZone> >::value == id)
			data = defData;
}



template<class T>struct ResultOk
{
	bool operator()(){return true;}
};

template<class T>struct ResultOk<BorderLower<T>>
{
	bool operator()(){return false;}
};
template<class T>struct ResultOk<BorderDefect<T>>
{
	bool operator()(){return false;}
};
template<>struct ResultOk<BrakStrobe2<Thickness>>
{
	bool operator()(){return false;}
};

	bool __res_ok__[TL::Length<label_message_list>::value];

	template<class T, int i = 0>struct __set_res_ok__;
	template<class Head, class Tail, int i>struct __set_res_ok__<Tlst<Head, Tail>, i>: __set_res_ok__<Tail, 1 + i>
	{
		__set_res_ok__(){__res_ok__[i] = ResultOk<typename __first__<Head>::Result>()();}
	};
	template<int i>struct __set_res_ok__<NullType, i>{};

    __set_res_ok__<label_message_list> __init_set_res_ok__;

bool TubeResult(char (&res)[App::count_zones], int count)
	{
		if(count > dimention_of(res)) count = dimention_of(res);
		for(int i = 0; i < count; ++i)
		{
			if(!__res_ok__[res[i]]) return false;
		}
		return true;
	}

char *__message__[ TL::Length<label_message_list>::value];

template<class T, int i = 0>struct __set_message__;
	template<class Head, class Tail, int i>struct __set_message__<Tlst<Head, Tail>, i>: __set_message__<Tail, 1 + i>
	{
		__set_message__(){__message__[i] = __status_label__<Head>::text();}
	};
	template<int i>struct __set_message__<NullType, i>{};

    __set_message__<label_message_list> __init_set_message__;



char *StatusText::operator()(int id, int &color, bool &visible)
{
	if(id < 0 || id >= TL::Length<label_message_list>::value) id = TL::IndexOf<label_message_list, Clr<Undefined>>::value;
	color = *__color__[id];

	 visible = !(TL::IndexOf<label_message_list, Clr<Undefined>>::value == id 
	   || TL::IndexOf<label_message_list, Clr<DeathZone> >::value == id);

	 if(id < TL::Length<label_message_list>::value) return __message__[id];

	return "";
}


bool CancelOperator(int id, unsigned &color)
{
	if(id < dimention_of(SelectMesageN::__bits__)) 
	{
		unsigned res = SelectMesageN::__bits__[id];
		static const int offs = TL::IndexOf<SelectMesageN::__selected_list__, Cancel<Projectionist>>::value;
		if(res & (1 << offs))
		{
			color = *__color__[offs];
			return true;
		}
	}
	return false;
}

bool IsBrackStrobe(int id)
{
	if(id < dimention_of(SelectMesageN::__bits__)) 
	{
		unsigned res = SelectMesageN::__bits__[id];
		static const int offs = TL::IndexOf<SelectMesageN::__selected_list__, BrakStrobe2<Thickness>>::value;
		if(res & (1 << offs)) return true;		
	}
	return false;
}

