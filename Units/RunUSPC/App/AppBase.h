#pragma once

#include "Base/tables.hpp"
#include "Base/TablesDefine.h"


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
		, oToShiftThe  //перекладка
		, oResult1   
		, oResult2   
		, oPowerBM   
		, oReserve   
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OutputBitTable";}
 };

struct ParametersBase
 {
	 typedef TL::MkTlst<
		  OutputBitTable
		  , Descriptor1730Table
	 >::Result one_row_table_list;

	 typedef TL::MkTlst<
		NullType
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
};
