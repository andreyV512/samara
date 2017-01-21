#pragma once
#include "Stored/StoredBase.h"

DEFINE_PARAM_WAPPER(ID, NumberPacket, unsigned, 0)

DEFINE_PARAM_WAPPER(ID, Alloy, unsigned, 0)
struct AlloysTable
{
	typedef TL::MkTlst<
		Alloy
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AlloyTable";}
};

DEFINE_PARAM_WAPPER(ID, DeliveryStatus, unsigned, 0)
struct DeliveryStatussTable
{
	typedef TL::MkTlst<
		DeliveryStatus
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DeliveryStatusTable";}
};

DEFINE_PARAM_WAPPER(ID, ProductCodeNumber, unsigned, 0)

DEFINE_PARAM_WAPPER(ID, NormativeDocument, unsigned, 0)

struct NormativeDocumentsTable
{
	typedef TL::MkTlst<
		NormativeDocument
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"NormativeDocumentsTable";}
};

DEFINE_PARAM_WAPPER(ID, Gang, unsigned, 0)
struct GangsTable
{
	typedef TL::MkTlst<
		Gang
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"GangTable";}
};

struct CurrentPacketTable
{
	typedef TL::MkTlst<
		ID<Operator>
		, ID<Alloy>
		, ID<DeliveryStatus>
		, ID<NormativeDocument>
		, ID<Gang>
		, ProductCodeNumber
		, NumberPacket
		, Standart		
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CurrentPacketTable";}
};

struct PacketBase
 {
	 typedef TL::MkTlst<		
		  OperatorsTable
		 , AlloysTable
		 , DeliveryStatussTable
		 , NormativeDocumentsTable
		 , CurrentPacketTable
		 , GangsTable
	 >::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	  wchar_t path[512];
	 const wchar_t *name();
	 static void Restore();
	 static void Save();
 };

struct NumberTubeTable
{
	typedef TL::MkTlst<
		NumberTube
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
};



