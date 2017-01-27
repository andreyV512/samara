#pragma once

#include "templates/typelist.hpp"
#include "App/TablesDefine.h"
#include "base/tables.hpp"
#include "App/AppBase.h"

DEFINE_PARAM(Hash, unsigned, -1)

struct Treshold
{
	typedef double type_value[App::count_zones];
	type_value value;	
	const wchar_t *name(){return L"Treshold";}
};

struct StoredThresholdsTable
{
	typedef TL::MkTlst<
		Hash
		, Treshold
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"StoredThresholdsTable";}
};

DEFINE_PARAM(Date_Time, COleDateTime, 0.0)
DEFINE_PARAM(LengthTube, int, 120)

DEFINE_PARAM_WAPPER2(ID, BorderKlass2 , Cross    , unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderDefect , Cross    , unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderKlass2 , Long     ,  unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderDefect , Long     ,  unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderNominal, Thickness, unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderAbove  , Thickness, unsigned, 0)
DEFINE_PARAM_WAPPER2(ID, BorderLower  , Thickness, unsigned, 0)

STR_PARAM(Operator, 128, L"Оператор");
DEFINE_PARAM_WAPPER(ID, Operator, unsigned, 0)

struct OperatorsTable
{
	typedef TL::MkTlst<
		Operator
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OperatorsTable";}
};

STR_PARAM(Alloy, 32, L"Сплав");
STR_PARAM(DeliveryStatus, 32, L"Состояние поставки");
STR_PARAM(NormativeDocument, 64, L"Нормативный документ")
STR_PARAM(Gang, 32, L"Смена");
STR_PARAM(ProductCodeNumber, 32, L"Шифр изделия");
STR_PARAM(NumberPacket, 16, L"00000");
STR_PARAM(Standart, 32, L"№СОП");
DEFINE_PARAM(ProtocolNumber, int, 0);

struct ProtocolsTable
{
	typedef TL::MkTlst<
		Alloy					//Сплав
		, DeliveryStatus		//Состояние поставки
		, NormativeDocument		//Нормативный документ
		, Gang					//Смена
		, ProductCodeNumber		//Шифр изделия
		, NumberPacket			//Номер партии
		, Standart				//№СОП
		, ProtocolNumber
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ProtocolsTable";}
};

DEFINE_PARAM_WAPPER(ID, ProtocolsTable, unsigned, 0)
struct StoredMeshureTable;
DEFINE_PARAM_WAPPER(ID, StoredMeshureTable, unsigned, 0)

STR_PARAM(NumberTube, 32, L"000");
struct TubesTable
{
	typedef TL::MkTlst<
		Date_Time	
		, NumberTube
		, ID<Operator>
		, ID<ProtocolsTable>
		, ID<StoredMeshureTable>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TubesTable";}
};

DEFINE_PARAM(Tube, unsigned, -1)
struct Status
{
	typedef unsigned char type_value[App::count_zones];
	type_value value;	
	const wchar_t *name(){return L"Status";}
};
struct StoredMeshureTable
{
	typedef TL::MkTlst<
	    LengthTube
		, ID<BorderKlass2 < Cross    > >
		, ID<BorderDefect < Cross    > >
		, ID<BorderKlass2 < Long     > >	
		, ID<BorderDefect < Long     > >	
		, ID<BorderNominal< Thickness> >
		, ID<BorderAbove  < Thickness> >
		, ID<BorderLower  < Thickness> >
		, DeadAreaMM0<Cross>
		, DeadAreaMM1<Cross>
		, DeadAreaMM0<Long>
		, DeadAreaMM1<Long>
		, DeadAreaMM0<Thickness>
		, DeadAreaMM1<Thickness>
		, Status
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"StoredMeshureTable";}
};

struct  MinMaxThickness
{
	typedef double type_value[App::count_zones];
	type_value value;	
	const wchar_t *name(){return L"MinMaxThickness";}
};

struct ThicknessTable
{
	typedef TL::MkTlst<
	   MinMaxThickness 
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThicknessTable";}
};

DEFINE_PARAM_ID(TubesTable, unsigned, 1)

DEFINE_PARAM(MinThicknessID, unsigned, 0)
DEFINE_PARAM(MaxThicknessID, unsigned, 0)

struct StoredThicknessTable
{
	typedef TL::MkTlst<
		ID<TubesTable>
		, MinThicknessID
		, MaxThicknessID
		, ID<ProtectiveThickeningTable >
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"StoredThicknessTable";}
};

struct StoredBase
 {
	 typedef TL::MkTlst<		
		 TubesTable
		 , OperatorsTable
		 , StoredThresholdsTable
		 , StoredMeshureTable
		 , ProtocolsTable
		 , ThicknessTable
		 , ProtectiveThickeningTable
		 , StoredThicknessTable
	 >::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 const wchar_t *name(){return L"StoredBase0";}
 };



