#pragma once
#include "App/AppBase.h"
#include "debug_tools/DebugMess.h"
//-----------------------------------------------------------------
typedef TL::MkTlst<
/*0*/		Clr<Undefined   >
/*1*/		, Clr<DeathZone>
/*2*/		, Clr<Nominal	>
/*3*/	    , Clr<BorderAbove<Thickness> >
/*4*/	    , Clr<BorderLower<Thickness> >
/*5*/	    , Clr<BorderKlass2<Long> > 
/*6*/	    , Clr<BorderDefect<Long> >
/*7*/	    , Clr<BorderKlass2<Cross>>
/*8*/	    , Clr<BorderDefect<Cross>>
/*9*/       , Clr<BrakStrobe2<Thickness>>
/*10*/      , Clr<Cancel<Projectionist>>
>::Result __first_color_list__;

typedef TL::MkTlst<	
	/*0*/		Clr<Undefined   >
	/*1*/		, Clr<DeathZone>
	/*2*/		, Clr<Nominal	>
	/*3*/	    , Clr<BorderAbove<Thickness> >
	/*4*/	    , Clr<BorderLower<Thickness> >
	/*5*/	    , Clr<BorderKlass2<Long> > 
	/*6*/	    , Clr<BorderDefect<Long> >
	/*7*/	    , Clr<BorderKlass2<Cross>>
	/*8*/	    , Clr<BorderDefect<Cross>>
	/*9*/       , Clr<BrakStrobe2<Thickness>>

	/*10*/  , Clr<Cancel<Projectionist>>
	/*11*/	, Clr< Nominal, Cancel<Projectionist>>
	/*12*/	, Clr<BorderAbove<Thickness>, Cancel<Projectionist>>
	/*13*/	, Clr<BrakStrobe2<Thickness>, Cancel<Projectionist> >
	/*14*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>>
	/*15*/	, Clr<BorderDefect<Cross>, BorderAbove<Thickness>>
	/*16*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>>
	/*17*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
	/*18*/	, Clr<BorderDefect<Long>, BorderAbove<Thickness>>
	/*19*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>>

	/*20*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	/*21*/	, Clr<BorderDefect<Cross>, BorderDefect<Long> >
	/*22*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
	/*23*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
	/*24*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	/*25*/  , Clr<BorderKlass2<Cross>, BorderAbove<Thickness>>
	/*26*/	, Clr<BorderLower<Thickness>, BorderKlass2<Cross>>
	/*27*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>
	/*28*/	, Clr<BorderKlass2<Long>, BorderAbove<Thickness>>
	/*29*/	, Clr<BorderLower<Thickness>, BorderKlass2<Long>>

	/*30*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>
	/*31*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long> >
	/*32*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
	/*33*/  , Clr<BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 
	/*34*/  , Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>
	/*35*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long> >
	/*36*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
	/*37*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
	/*38*/  , Clr<BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	/*39*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross> >

	/*40*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>>   
	/*41*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
	/*42*/  , Clr<BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 
	/*43*/	, Clr<BorderLower<Thickness>, Cancel<Projectionist> >
	/*44*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*45*/	, Clr<BorderKlass2<Long> , Cancel<Projectionist>> 
	/*46*/	, Clr<BorderDefect<Long> , Cancel<Projectionist>>
	/*47*/	, Clr<BorderKlass2<Cross>, Cancel<Projectionist>>
	/*48*/	, Clr<BorderDefect<Cross>, Cancel<Projectionist>>		
	/*49*/	, Clr<BorderDefect<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>

	/*50*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, Cancel<Projectionist>>
	/*51*/	, Clr<BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*52*/	, Clr<BorderDefect<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*53*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, Cancel<Projectionist>>
	/*54*/	, Clr<BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*55*/	, Clr<BorderDefect<Cross>, BorderDefect<Long>, Cancel<Projectionist>>
	/*56*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	/*57*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, Cancel<Projectionist>> 
	/*58*/  , Clr<BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*59*/  , Clr<BorderKlass2<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>

	/*60*/	, Clr<BorderLower<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>
	/*61*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>
	/*62*/	, Clr<BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*63*/	, Clr<BorderLower<Thickness>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*64*/	, Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*65*/	, Clr<BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*66*/  , Clr<BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	/*67*/  , Clr<BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>> 
	/*68*/  , Clr<BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*69*/  , Clr< BorderDefect<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>

	/*70*/  , Clr< BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*71*/  , Clr< BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, Cancel<Projectionist>>    
	/*72*/  , Clr< BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*73*/  , Clr< BorderDefect<Long>, BorderKlass2<Cross>, Cancel<Projectionist>>
	/*74*/  , Clr< BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	/*75*/  , Clr< BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, Cancel<Projectionist>>      
	/*76*/  , Clr< BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>> 
	/*77*/	, Clr<BrakStrobe2<Thickness>, BorderAbove<Thickness> >
	/*78*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness> >
	/*79*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>>

	/*80*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long> > 
	/*81*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long> >
	/*82*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>>
	/*83*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>>
	/*84*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderAbove<Thickness>>
	/*85*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>>
	/*86*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>>
	/*87*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderAbove<Thickness>>
	/*88*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>>
	/*89*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>

	/*90*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long> >
	/*91*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>>   
	/*92*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>> 
	/*93*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	/*94*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderAbove<Thickness>>
	/*95*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>>
	/*96*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>>
	/*97*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Long>, BorderAbove<Thickness>>
	/*98*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Long>>
	/*99*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>>

	/*100*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long> >
	/*101*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>   
	/*102*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>> 
	/*103*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>>
	/*104*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long> >
	/*105*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>>
	/*106*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>>    
	/*107*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>>
	/*108*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross> >
	/*109*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>> 

	/*110*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>>      
	/*111*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>> 
	/*112*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, Cancel<Projectionist> >
	/*113*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*114*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long> , Cancel<Projectionist>> 
	/*115*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long> , Cancel<Projectionist>>
	/*116*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>
	/*117*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, Cancel<Projectionist>>
	/*118*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*119*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>, Cancel<Projectionist>>

	/*120*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*121*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*122*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>, Cancel<Projectionist>>
	/*123*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*124*/	, Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, Cancel<Projectionist>>
	/*125*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	/*126*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, Cancel<Projectionist>> 
	/*127*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderDefect<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*128*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*129*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>

	/*130*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, Cancel<Projectionist>>
	/*131*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*132*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*133*/	, Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*134*/	, Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*135*/  , Clr<BrakStrobe2<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	/*136*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>> 
	/*137*/  , Clr<BrakStrobe2<Thickness>, BorderLower<Thickness>, BorderAbove<Thickness>, BorderKlass2<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*138*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, Cancel<Projectionist>>
	/*139*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderAbove<Thickness>, Cancel<Projectionist>>

	/*140*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, Cancel<Projectionist>>    
	/*141*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Cross>, BorderKlass2<Long>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>>
	/*142*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, Cancel<Projectionist>>
	/*143*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderAbove<Thickness>, Cancel<Projectionist>>   
	/*144*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, Cancel<Projectionist>>      
	/*145*/  , Clr<BrakStrobe2<Thickness>, BorderDefect<Long>, BorderKlass2<Cross>, BorderLower<Thickness>, BorderAbove<Thickness>, Cancel<Projectionist>> 
	//------------------------------------------------------------------------------
	>::Result label_message_list;
//-------------------------------------------------------------------
template<class O>struct __status_label__
{
	typedef typename O::__template_must_be_overridded__ noused;
};

template<>struct __status_label__<NullType>
{
	static char *text(){return "NO_MESSAGE";}
};

#define STATUS_LABEL(O, txt)template<>struct __status_label__<O>\
{\
	static char *text(){return txt;};\
};

namespace
{
	struct __data_text__
	{
		int id;
		char *text;
		int color;
		bool visibleVal;
	};
	template<class T>struct ValueVisible
	{
		static const bool value = true;
	};
	template<>struct ValueVisible<Clr<Undefined>>
	{
		static const bool value = false;
	};
	template<>struct ValueVisible<Clr<DeathZone>>
	{
		static const bool value = false;
	};
	template<class T>struct __first_color__;

	template<class List>struct __xel__;
	template<class A, class B, class C, class D, class E, class F, class Tail>struct __xel__<Tlst<Clr<A, B, C, D, E, F>, Tail>>
	{
		typedef Tlst<Clr<A, B, C, D, E, F>, typename __xel__<Tail>::Result> Result;
	};
	template<class Head, class Tail>struct __xel__<Tlst<Head, Tail>>
	{
		typedef typename __xel__<Tail>::Result Result;
	};
	template<>struct __xel__<NullType>
	{
		typedef NullType Result;
	};

	template<class  _0, class _1, class _2, class _3, class _4,class _5>struct __first_color__<Clr<_0, _1, _2, _3, _4,_5>>
	{
		typedef Clr<_0, _1, _2, _3, _4, _5> O;
		typedef typename TL::_if<(TL::Length<typename __xel__<__first_color_list__>::Result>::value > TL::IndexOf<label_message_list, O>::value), O, Clr<_0>>::Result Result;
	};
	
	template<class O, class P>struct __select__
	{
		bool operator()(P *p)
		{
			if(TL::IndexOf<label_message_list, O>::value == p->id)
			{
				p->text = __status_label__<O>::text();
				p->color = Singleton<ColorTable>::Instance().items.get<typename __first_color__<O>::Result>().value;
				p->visibleVal = ValueVisible<O>::value;
				return false;
			}
			return true;
		}
	};
}

STATUS_LABEL(Clr<Undefined>, "\"результат не определён\"")
STATUS_LABEL(Clr<DeathZone>, "\"мёртвая зона\"")

STATUS_LABEL(Clr<Nominal>, "\"норма\"")
STATUS_LABEL(Clr<BrakStrobe2<Thickness>>, "\"расслоение\"")

STATUS_LABEL(Clr<BorderAbove<Thickness>>, "\"толщина больше нормы\"") 
STATUS_LABEL(Clr<BorderLower<Thickness>>, "\"толщина меньше нормы\"")

STATUS_LABEL(Clr<Cancel<Projectionist>>, "\"отмена\"")
//---------------------------------------------------------------------------
template<class O, class P>struct __set_color_bar__;

namespace
{
	struct __data_color__
	{
		int id;
		unsigned &color;
		double &data;
		double defData;
		__data_color__(int id, unsigned &color, double &data, double defData)
			: id(id)
		    , color(color)
		    , data(data)
			, defData(defData)
		{}
	};
}

template<class O, class P>struct __set_color_bar__
{
	int &color;
	__set_color_bar__() : color(Singleton<ColorTable>::Instance().items.get<O>().value){}
	bool operator()(P *p)
    {
		if(TL::IndexOf<ColorTable::items_list, O>::value == p->id)
		{
		     p->color = color;
             return false;
		}
		return true;
    }
};

template<class T>struct GetFirst;
template<template<class, class, class, class, class,class>class X, class A, class B, class C, class D, class E, class F>struct GetFirst<X<A, B, C, D, E,F> >
{
	typedef X<A, NullType, NullType, NullType, NullType, NullType> Result;
};
template<class O, class P>struct __set_color_bar_next__
{
	bool operator()(P *p)
    {
		if(TL::IndexOf<label_message_list, O>::value == p->id)
		{
			p->color = Singleton<ColorTable>::Instance().items.get<typename GetFirst<O>::Result>().value;
             return false;
		}
		return true;
    }
};

#define COLOR_DATA(O)template<class P>struct __set_color_bar__<O, P>\
{\
	int &color;\
	__set_color_bar__() \
        : color(Singleton<ColorTable>::Instance().items.get<O>().value)\
        {}\
	bool operator()(P *p)\
    {\
        if(TL::IndexOf<label_message_list, O>::value == p->id)\
		{\
		     p->color = color;\
			 p->data = p->defData;\
             return false;\
		}\
		return true;\
    }\
};

COLOR_DATA(Clr<Undefined>)
COLOR_DATA(Clr<DeathZone>)

#define TXT(a, b) a##_##b
#define STR(a, b) a<b>
#define PARAM_STR(a, b) STR(a, b)
#define PARAM_TXT(a, b) TXT(a, b)

#define STATUS_LABEL_1(a)template<>struct __status_label__<Clr<PARAM_STR##a>>\
{\
	static char *text(){return PARAM_TXT##a;}\
};

#define STATUS_LABEL_2(a, b)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b>>\
{\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b;}\
};

#define STATUS_LABEL_3(a, b, c)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c>>\
{\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c;};\
};

#define STATUS_LABEL_4(a, b, c, d)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>\
{\
	static const int ID = TL::IndexOf<label_message_list, Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d>>::value;\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d;}\
};

#define STATUS_LABEL_5(a, b, c, d, e)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d, PARAM_STR##e>>\
{\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d##PARAM_TXT##e;}\
};
#define STATUS_LABEL_6(a, b, c, d, e, f)template<>struct __status_label__<Clr<PARAM_STR##a, PARAM_STR##b, PARAM_STR##c, PARAM_STR##d, PARAM_STR##e, PARAM_STR##f>>\
{\
	static char *text(){return PARAM_TXT##a##PARAM_TXT##b##PARAM_TXT##c##PARAM_TXT##d##PARAM_TXT##e##PARAM_TXT##f;}\
};

//-------------------------------
#define BorderDefect_Cross "\"поперечный дефект\""
#define BorderDefect_Long "\"продольный дефект\""

#define BorderKlass2_Cross "\"поперечный 2 класс\""
#define BorderKlass2_Long "\"продольный 2 класс\""

#define BorderLower_Thickness "\"толщина меньше нормы\""
#define BorderAbove_Thickness "\"толщина больше нормы\""

#define BrakStrobe2_Thickness "\"расслоение\""

#define Cancel_Projectionist "\"отмена\""

STATUS_LABEL_2((BorderAbove, Thickness), (Cancel, Projectionist))
template<>struct __status_label__<Clr<Nominal, Cancel<Projectionist>>>
{
	static char *text(){return Cancel_Projectionist;}
};

STATUS_LABEL_2((BrakStrobe2, Thickness), (Cancel, Projectionist))

STATUS_LABEL_1((BorderDefect, Cross))
STATUS_LABEL_1((BorderDefect, Long))

STATUS_LABEL_1((BorderKlass2, Cross))
STATUS_LABEL_1((BorderKlass2, Long))

STATUS_LABEL_2((BorderLower, Thickness), (BorderAbove, Thickness))

STATUS_LABEL_2((BorderDefect, Cross), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderDefect, Cross), (BorderLower, Thickness))
STATUS_LABEL_3((BorderDefect, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))

STATUS_LABEL_2((BorderDefect, Long), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderDefect, Long), (BorderLower, Thickness))
STATUS_LABEL_3((BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness))

STATUS_LABEL_2((BorderDefect, Cross), (BorderDefect, Long))

STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness))
STATUS_LABEL_4((BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
//---------------------------------------------------------------------------
STATUS_LABEL_2((BorderKlass2, Cross), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderLower, Thickness), (BorderKlass2, Cross))
STATUS_LABEL_3( (BorderLower, Thickness), (BorderAbove, Thickness),(BorderKlass2, Cross))
				
STATUS_LABEL_2((BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderLower, Thickness), (BorderKlass2, Long))
STATUS_LABEL_3((BorderLower, Thickness), (BorderAbove, Thickness), (BorderKlass2, Long))
			
STATUS_LABEL_2((BorderKlass2, Cross), (BorderKlass2, Long))
				
STATUS_LABEL_3((BorderKlass2, Cross), (BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderLower, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_4((BorderLower, Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
//--------------------
STATUS_LABEL_2((BorderDefect, Cross), (BorderKlass2, Long))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness))
STATUS_LABEL_4((BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_2((BorderDefect, Long), (BorderKlass2, Cross))												
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (BorderAbove, Thickness))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness))
STATUS_LABEL_4((BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))

STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderLower , Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness))
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderKlass2, Long)) 
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderDefect, Long))
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderKlass2, Cross))
STATUS_LABEL_2((BrakStrobe2, Thickness), (BorderDefect, Cross))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderLower, Thickness))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderLower, Thickness))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (BorderAbove, Thickness))   
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness)) 
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderKlass2, Cross), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderKlass2, Cross))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderKlass2, Long))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Long))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long), (BorderAbove, Thickness))   
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (BorderAbove, Thickness))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness), (BorderAbove, Thickness))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (BorderAbove, Thickness))   
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness))      
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness), (BorderAbove, Thickness))

STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderLower , Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderKlass2, Long), (Cancel, Projectionist)) 
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Long), (Cancel, Projectionist))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_3((BrakStrobe2, Thickness), (BorderDefect, Cross), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderLower, Thickness), (Cancel, Projectionist))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderLower, Thickness), (Cancel, Projectionist))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (Cancel, Projectionist))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (BorderAbove, Thickness), (Cancel, Projectionist))   
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness), (Cancel, Projectionist)) 
STATUS_LABEL_6((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderKlass2, Cross), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderKlass2, Long), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long), (BorderAbove, Thickness), (Cancel, Projectionist))   
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_6((BrakStrobe2, Thickness), (BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness), (Cancel, Projectionist))
STATUS_LABEL_6((BrakStrobe2, Thickness), (BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (BorderAbove, Thickness), (Cancel, Projectionist))   
STATUS_LABEL_5((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness), (Cancel, Projectionist))      
STATUS_LABEL_6((BrakStrobe2, Thickness), (BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))


STATUS_LABEL_2((BorderLower , Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BorderLower , Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_2((BorderKlass2, Long), (Cancel, Projectionist)) 
STATUS_LABEL_2((BorderDefect, Long), (Cancel, Projectionist))
STATUS_LABEL_2((BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_2((BorderDefect, Cross), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Cross), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Cross), (BorderLower, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Long), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Long), (BorderLower, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Cross), (BorderDefect, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderDefect, Long), (BorderAbove, Thickness), (Cancel, Projectionist))   
STATUS_LABEL_4((BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness), (Cancel, Projectionist)) 
STATUS_LABEL_5((BorderDefect, Cross), (BorderDefect, Long), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BorderKlass2, Cross), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BorderLower , Thickness), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_4((BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_3((BorderKlass2, Long), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BorderLower , Thickness), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_3((BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderKlass2, Cross), (BorderKlass2, Long), (BorderAbove, Thickness), (Cancel, Projectionist))   
STATUS_LABEL_4((BorderLower , Thickness), (BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_5((BorderLower , Thickness), (BorderAbove, Thickness), (BorderKlass2, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Cross), (BorderKlass2, Long), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderKlass2, Long), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness), (Cancel, Projectionist))
STATUS_LABEL_5((BorderDefect, Cross), (BorderKlass2, Long), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))
STATUS_LABEL_3((BorderDefect, Long), (BorderKlass2, Cross), (Cancel, Projectionist))
STATUS_LABEL_4((BorderDefect, Long), (BorderKlass2, Cross), (BorderAbove, Thickness), (Cancel, Projectionist))   
STATUS_LABEL_4((BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness), (Cancel, Projectionist))      
STATUS_LABEL_5((BorderDefect, Long), (BorderKlass2, Cross), (BorderLower, Thickness), (BorderAbove, Thickness), (Cancel, Projectionist))


#undef COLOR_DATA
#undef STATUS_LABEL
#undef STATUS_LABEL_2
#undef STATUS_LABEL_3
#undef STATUS_LABEL_4
#undef STATUS_LABEL_5
#undef STATUS_LABEL_6
#undef TXT
#undef STR
#undef PARAM_STR
#undef PARAM_TXT

#undef BorderDefect_Cross
#undef BorderDefect_Long 
#undef BorderKlass2_Cross 
#undef BorderKlass2_Long 
#undef BorderLower_Thickness
#undef BorderAbove_Thickness
//------------------------------------------------------------------------------------------------



