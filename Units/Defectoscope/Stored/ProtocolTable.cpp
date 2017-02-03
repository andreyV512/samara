#include "stdafx.h"
#include "Stored/ProtocolTable.h"
#include "Stored/PacketBase.h"
#include "Base/ExpressBase.hpp"

namespace
{
	template<class O, class P>struct __set_data_table__
	{
		void operator()(O *o)
		{
			o->value = Singleton<O>::Instance().value;
		}
	};

	//template<class P>struct __set_data_table__<ProtocolNumber, P>
	//{
	//	void operator()(ProtocolNumber *o)
	//	{
	//		o->value = 0;
	//	}
	//};
}

unsigned GetProtocolID(CBase &b)
{
	ProtocolsTable t;
	TL::foreach<ProtocolsTable::items_list, __set_data_table__>()(&t.items);
	//unsigned id = Select<ProtocolsTable>(b).eq_all<ProtocolsTable::items_list>(&t.items).Execute();
	TL::Factory<TL::MkTlst<ProtocolNumber>::Result> param;
	unsigned id = Select<ProtocolsTable>(b).eq<NumberPacket>(t.items.get<NumberPacket>().value).Execute(param);
	if(0 == id)
	{
		Insert_Into<ProtocolsTable>(t, b).Execute();
		id = Select<ProtocolsTable>(b).eq_all<ProtocolsTable::items_list>(&t.items).Execute();
	}
	else
	{
		t.items.get<ProtocolNumber>().value = param.get<ProtocolNumber>().value;
		UpdateWhere<ProtocolsTable>(t, b).ID(id).Execute();
	}
	return id;
}

bool TestNumberTableExist(wchar_t *number)
{
	StoredBase parameters;

	CExpressBase base(
		parameters.name()
		, CreateDataBase<StoredBase::type_list, NullType, MSsql>()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		unsigned id = GetProtocolID(base);
		NumberTube nt;
		nt.value = number;
		return 0 != Select<TubesTable>(base)
			.eq<NumberTube>(nt.value)
			.eq<ID<ProtocolsTable>>(id)
			.Execute();
	}
	return false;
}