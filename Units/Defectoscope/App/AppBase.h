#pragma once
#include "DiffApp/App.h"
#include "Base/tables.hpp"
#include "App/TablesDefine.h"
//-------------------------------------------------------------------------------------------
void TrimTypeList(wchar_t *, wchar_t *);
//-------------------------------------------------------------------------------------------
DEFINE_PARAM(CurrentID, int, 1)

struct CurrentParametersTable
{
	typedef TL::MkTlst<
		CurrentID
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CurrentParametersTable";}
};
//------------------------------------------------------------------------------------------
template<class>struct BorderKlass2;
template<class>struct BorderDefect;

template<class>struct BorderAbove;
template<class>struct BorderLower;
template<class>struct BorderNominal;

DEFINE_ARRAY_PARAM_WAPPER(BorderKlass2, Long, double, App::count_zones, 20)
DEFINE_ARRAY_PARAM_WAPPER(BorderDefect, Long, double, App::count_zones, 30)

DEFINE_ARRAY_PARAM_WAPPER(BorderKlass2, Cross, double, App::count_zones, 40)
DEFINE_ARRAY_PARAM_WAPPER(BorderDefect, Cross, double, App::count_zones, 60)

DEFINE_ARRAY_PARAM_WAPPER(BorderAbove  , Thickness, double, App::count_zones, 3.0)
DEFINE_ARRAY_PARAM_WAPPER(BorderLower  , Thickness, double, App::count_zones, 2.0)
DEFINE_ARRAY_PARAM_WAPPER(BorderNominal, Thickness, double, App::count_zones, 12.0)

struct ThresholdsTable
{
	typedef TL::MkTlst<
		BorderKlass2<Long>
		, BorderDefect<Long>
		, BorderKlass2<Cross>
		, BorderDefect<Cross>
		 , BorderAbove  <Thickness>
		 , BorderLower  <Thickness>
		 , BorderNominal<Thickness>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThresholdsTable";}
};

//template<class>struct BrakStrobe2;
//DEFINE_PARAM_WAPPER(BrackStrobe2, Thickness, double, 1)
//DEFINE_PARAM_WAPPER(AxesYMax, Thickness, double, 25)

template<class T>struct BrakStrobe2;
DEFINE_PARAM_WAPPER(BrakStrobe2, Thickness, double, 1)

struct BrackStrobe2Table
{
	typedef TL::MkTlst<
		 BrakStrobe2<Thickness>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"BrackStrobe2Table";}
};

//----------------------------------------------------------------------------------------
DEFINE_PARAM(CommunicationRemoveUnit, int, 0)
DEFINE_PARAM(CounterTubesStored, int, 0)

struct DifferentOptionsTable
{
	typedef TL::MkTlst<
		CommunicationRemoveUnit
		, CounterTubesStored
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DifferentOptionsTable";}
};
DEFINE_PARAM(OffsetAxesX, int, 100)
DEFINE_PARAM(FrameWidth, int, 400)
DEFINE_PARAM(MaxAxesY, double, 8191)
DEFINE_PARAM(MinAxesY, double, -8192)
struct GraphicSignalOptionsTable
{
	typedef TL::MkTlst<
		OffsetAxesX
		, FrameWidth
		, MaxAxesY
		, MinAxesY
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"GraphicSignalOptionsTable";}
};
struct PointsOptionsTable
{
	typedef TL::MkTlst<
		MaxAxesY
		, MinAxesY
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"PointsOptionsTable";}
};
//----------------------------------------------------------------------------------
template<class _0=NullType, class _1=NullType, class _2=NullType, class _3=NullType, class _4=NullType, class _5=NullType>struct Clr
{
	typedef typename TL::MkTlst<_0, _1, _2, _3, _4, _5>::Result items_list;
};

template<class T>struct GetItemsList;

template<template<class, class, class, class, class,class>class W, class _0, class _1, class _2, class _3, class _4, class _5>struct	GetItemsList<W<_0, _1, _2, _3, _4,_5>>
{
	typedef typename TL::MkTlst<_0, _1, _2, _3, _4, _5>::Result Result;
};

struct Undefined{};
struct Nominal{};
struct DeathZone{typedef NullType items_list;};
template<class>struct Cancel;
struct Projectionist{};

#define	JOIN2(a, b) a##,##b
DEFINE_WAPPER(Clr<Undefined>, int, 0xff555555)
DEFINE_WAPPER(Clr<Nominal  >, int, 0xff00ff00)
DEFINE_WAPPER(Clr<DeathZone>, int, 0xff333333) 
DEFINE_WAPPER(Clr<BorderAbove<Thickness>  >, int, 0xff0000ff)
DEFINE_WAPPER(Clr<BorderLower<Thickness>  >, int, 0xffff0000)
DEFINE_WAPPER(Clr<BorderKlass2<Long>      >, int, 0xffffff00)
DEFINE_WAPPER(Clr<BorderDefect<Long>      >, int, 0xffff0000)
DEFINE_WAPPER(Clr<BorderKlass2<Cross>     >, int, 0xffffff00)
DEFINE_WAPPER(Clr<BorderDefect<Cross>     >, int, 0xffff0000)
DEFINE_WAPPER( Clr<BrakStrobe2<Thickness>>, int, 0xffff8000)
DEFINE_WAPPER( Clr<Cancel<Projectionist>>, int, 0xff56ff32)
#undef	JOIN2


struct ColorTable
{
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
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ColorTable";}
};

template<class T>inline int StatusId()
{
	return TL::IndexOf<ColorTable::items_list, T>::value;
}

template<class T>struct Stat
{
	static const unsigned value = TL::IndexOf<ColorTable::items_list, T>::value;
};
//-------------------------------------------------------------------------------------- 
DEFINE_PARAM(ReferenceOffset1, unsigned, 4835)
struct AdditionalSettingsTable
{
	typedef TL::MkTlst<	
		ReferenceOffset1
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AdditionalSettingsTable";}
};
//--------------------------------------------------------------------------------------
 template<class>struct DeadAreaMM0;
 template<class>struct DeadAreaMM1;
 DEFINE_PARAM_WAPPER(DeadAreaMM0, Cross, int, 200)
 DEFINE_PARAM_WAPPER(DeadAreaMM1, Cross, int, 200)

 DEFINE_PARAM_WAPPER(DeadAreaMM0, Long, int, 200)
 DEFINE_PARAM_WAPPER(DeadAreaMM1, Long, int, 200)

 DEFINE_PARAM_WAPPER(DeadAreaMM0, Thickness, int, 200)
 DEFINE_PARAM_WAPPER(DeadAreaMM1, Thickness, int, 200)
 
 struct DeadAreaTable
 {
	typedef TL::MkTlst<
		DeadAreaMM0<Cross>
		, DeadAreaMM1<Cross>
		, DeadAreaMM0<Long>
		, DeadAreaMM1<Long>
		, DeadAreaMM0<Thickness>
		, DeadAreaMM1<Thickness>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DeadAreaTable";}
 };
//----------------------------------------------------------------------------------------------------
 template<class>struct MedianFiltreWidth;
 template<class>struct MedianFiltreOn;

 DEFINE_PARAM_WAPPER(MedianFiltreWidth, Cross, int, 5)
 DEFINE_PARAM_WAPPER(MedianFiltreOn, Cross, bool, true)

 DEFINE_PARAM_WAPPER(MedianFiltreWidth, Long, int, 5)
 DEFINE_PARAM_WAPPER(MedianFiltreOn, Long, bool, true)

 DEFINE_PARAM_WAPPER(MedianFiltreWidth, Thickness, int, 5)
 DEFINE_PARAM_WAPPER(MedianFiltreOn, Thickness, bool, true)
 
 struct MedianFiltreTable
 {
	 typedef TL::MkTlst<
		 MedianFiltreWidth<Cross>
		 , MedianFiltreOn<Cross> 
		 , MedianFiltreWidth<Long>
		 , MedianFiltreOn<Long> 
		 , MedianFiltreWidth<Thickness>
		 , MedianFiltreOn<Thickness> 
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"MedianFiltreTable";}
 };
 //---------------------------------------------------------------------------------------------------------
 DEFINE_PARAM(NamePlate1730, int, 3)
 struct NamePlate1730ParametersTable
 {
	 typedef TL::MkTlst<
		 NamePlate1730
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"NamePlate1730ParametersTable";}
 };
//-------------------------------------------------------------------------------------------------------
struct AxesTable;
struct ProtectiveThickeningTable;
DEFINE_PARAM_ID(ThresholdsTable            , int, 1)
DEFINE_PARAM_ID(DeadAreaTable			   , int, 1)
DEFINE_PARAM_ID(AxesTable	   , int, 1)
DEFINE_PARAM_ID(MedianFiltreTable, int, 1)
DEFINE_PARAM_ID(BrackStrobe2Table, int, 1)
STR_PARAM(NameParam, 128, L"NONAME")
DEFINE_PARAM_ID(ProtectiveThickeningTable, int, 1)
 struct ParametersTable
 {
	typedef TL::MkTlst<
		ID<ThresholdsTable>
		, ID<DeadAreaTable			   	>
		, ID<AxesTable	   	>
		, ID<MedianFiltreTable>
		, ID<BrackStrobe2Table>
		, ID<ProtectiveThickeningTable>
		, NameParam
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ParametersTable";}
 };
//--------------------------------------------------------------------------------------------------------
DEFINE_PARAM(iÑontrolÑircuits  , unsigned, 1 << 0)
DEFINE_PARAM(iCycle            , unsigned, 1 << 1)
DEFINE_PARAM(iReady            , unsigned, 1 << 2)
DEFINE_PARAM(iControl          , unsigned, 1 << 3)
DEFINE_PARAM(iBase             , unsigned, 1 << 4)
DEFINE_PARAM(iReserve          , unsigned, 1 << 5)

struct InputBitTable
 {
	typedef TL::MkTlst<
		iÑontrolÑircuits
		, iCycle          
		, iReady          
		, iControl        
		, iBase           
		, iReserve        
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputBitTable";}
 };

DEFINE_PARAM(oWork            , unsigned, 1 << 0)
DEFINE_PARAM(oToShiftThe      , unsigned, 1 << 1)
DEFINE_PARAM(oResult1         , unsigned, 1 << 2)
DEFINE_PARAM(oResult2         , unsigned, 1 << 3)
DEFINE_PARAM(oPowerBM         , unsigned, 1 << 4)
DEFINE_PARAM(oReserve         , unsigned, 1 << 5)

struct OutputBitTable
 {
	typedef TL::MkTlst<
		oWork      
		, oToShiftThe  //ïåðåêëàäêà
		, oResult1   
		, oResult2   
		, oPowerBM   
		, oReserve   
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OutputBitTable";}
 };

 DEFINE_PARAM(Descriptor1730, unsigned, 0)
struct Descriptor1730Table
 {
	typedef TL::MkTlst<
		Descriptor1730
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"Descriptor1730Table";}
 };
 //--------------------------------------------------------------------------------------------------------
///\brief Ñìåùåíèå èçìåðèòåëüíûõ äàò÷èêîâ
template<int N>struct Offset;
DEFINE_PARAM_NUM(Offset, 0, int, 430)
DEFINE_PARAM_NUM(Offset, 1, int, 380)
DEFINE_PARAM_NUM(Offset, 2, int, 330)
DEFINE_PARAM_NUM(Offset, 3, int, 280)
DEFINE_PARAM_NUM(Offset, 4, int, 150)
DEFINE_PARAM_NUM(Offset, 5, int, 100)
DEFINE_PARAM_NUM(Offset, 6, int, 50)
DEFINE_PARAM_NUM(Offset, 7, int, 0)

struct OffsetsTable
{
	typedef TL::CreateNumList<Offset, 0, 7>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OffsetsTable";}
};
//-----------------------------------------------------------------------------------------------------------
template<class T>struct AxesYMin;
template<class T>struct AxesYMax;
DEFINE_PARAM_WAPPER(AxesYMin, Long, double, 0)
DEFINE_PARAM_WAPPER(AxesYMax, Long, double, 100)
DEFINE_PARAM_WAPPER(AxesYMin, Cross, double, 0)
DEFINE_PARAM_WAPPER(AxesYMax, Cross, double, 100)
DEFINE_PARAM_WAPPER(AxesYMin, Thickness, double, 0)
DEFINE_PARAM_WAPPER(AxesYMax, Thickness, double, 25)

struct AxesTable
{
	typedef TL::MkTlst<
		AxesYMin<Long>
		, AxesYMax<Long>
		, AxesYMin<Cross>
		, AxesYMax<Cross>
		, AxesYMin<Thickness>
		, AxesYMax<Thickness>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AxesTable";}
};
//-----------------------------------------------------------------------------------------------------------
template<class T>struct OnTheJob;
DEFINE_PARAM_WAPPER(OnTheJob, Cross    , bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, Long     , bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, Thickness, bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, ViewInterrupt, bool, true)

struct OnTheJobTable
{
	typedef TL::MkTlst<
		OnTheJob<Cross    >
		, OnTheJob<Long     >
		, OnTheJob<Thickness>
		, OnTheJob<ViewInterrupt>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OnTheJobTable";}
};
//-----------------------------------------------------------------------------------------------------------
wchar_t *Trim(wchar_t *);
struct TestParam0
{
	typedef int type_value;
	type_value value;\
	const type_value default_value;
	const wchar_t *name(){return L"DefectCrossAbove";}
	TestParam0(): value(5), default_value(5) {}
};
struct TestParam1
{
	typedef int type_value;
	type_value value;\
	const type_value default_value;
	const wchar_t *name(){return L"DefectLongAbove";}
	TestParam1(): value(5), default_value(5) {}
};
struct TestTable
{
	typedef TL::MkTlst<
		TestParam0
		, TestParam1
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TestTable";}
};
//-------------------------------------------------------------------------------------------
STR_PARAM(PathUSPC, 255, L"")
struct PathUSPCTable
{
	typedef TL::MkTlst<
		PathUSPC
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"PathUSPCTable";}
};
//------------------------------------------------------------------------------------------------------------
DEFINE_PARAM(N0, unsigned, 5)
DEFINE_PARAM(N1, unsigned, 96)
DEFINE_PARAM(N2, unsigned, 139)
DEFINE_PARAM(N3, unsigned, 237)

struct ProtectiveThickeningTable
{
	typedef TL::MkTlst<
	  N0, N1, N2, N3 
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ProtectiveThickeningTable";}
};
//-----------------------------------------------------------------
 struct ParametersBase
 {
	 typedef TL::MkTlst<
		  NamePlate1730ParametersTable	  		
		 , PointsOptionsTable			
		 , GraphicSignalOptionsTable	   
		 , AdditionalSettingsTable
		 , ColorTable
		 , InputBitTable
		 , OutputBitTable
		 , Descriptor1730Table
		 , OffsetsTable
		 , OnTheJobTable
		 , TestTable
		 , PathUSPCTable
	 >::Result one_row_table_list;

	 typedef TL::MkTlst<
		  CurrentParametersTable		 
		 , ParametersTable			   
		 , ThresholdsTable			  
		 , DeadAreaTable	
		 , MedianFiltreTable
		 , AxesTable
		 , BrackStrobe2Table
		 , ProtectiveThickeningTable
	 >::Result multy_row_table_list;

	 typedef TL::MkTlst<
		  multy_row_table_list
		 , one_row_table_list
	 >::Result multy_type_list; 

	 typedef TL::MultyListToList<multy_type_list>::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 wchar_t path[512];
	 const wchar_t *name();
 };

struct AppBase
{
	void Init();
	static void InitTypeSizeTables(CBase &);
};

template<class T>int CurrentId()
{
	  return Singleton<ParametersTable>::Instance().items.get<T>().value;
}

template<class T>void UpdateId(CBase &base, int num)
{
   CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
   Select<CurrentParametersTable>(base).ID(1).Execute(current);
   ParametersTable &t = Singleton<ParametersTable>::Instance();
   t.items.get<T>().value = num;
   UpdateWhere<ParametersTable>(t, base).ID(current.items.get<CurrentID>().value).Execute();
}

template<class T>int CountId(CBase &base, int num)
{
	ADODB::_RecordsetPtr rec;
	Select<ParametersTable>(base).eq<T>(num).Execute(rec);
	int i = 0;
	while (!rec->EndOfFile) 
	{			
		++i;
		rec->MoveNext(); 
	}
	return i;
}








