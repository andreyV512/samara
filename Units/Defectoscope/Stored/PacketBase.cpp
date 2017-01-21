#include "stdafx.h"
#include "Stored/PacketBase.h"

const wchar_t *PacketBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = (int)wcslen(path) - 4;
	wcscpy(&path[len], L"Packet");
	return path;
}

namespace
{
	template<class O, class P>struct __restore__
	{
		void operator()(O *o, P *)
		{
			Singleton<O>::Instance().value = o->value;
		}
	};

	template<class T>struct Table{typedef typename T::__template_must_be_overridded__ noused;};
#define TABLE(n)template<>struct Table<n>{typedef n##sTable Result;};
	TABLE(Operator)
	TABLE(Alloy)
	TABLE(DeliveryStatus)
	TABLE(NormativeDocument)
	TABLE(Gang)
#undef TABLE

	template<class X, class P>struct __restore__<ID<X>, P>
	{
		typedef ID<X> O;
		void operator()(O *o, P *base)
		{
			typedef typename Table<X>::Result T;
			T t;
			Select<T>(*base).ID(o->value).Execute(t);
			Singleton<X>::Instance().value = t.items.get<X>().value;
		}
	};
	template<class P>struct __restore__<CurrentPacketTable, P>
	{
		void operator()(CurrentPacketTable *, P *){}
	};

	template<class O, class P>struct __save__
	{
		void operator()(P *p)
		{
			p->set<O>(Singleton<O>::Instance().value);
		}
	};
	template<class X, class P>struct __save__<ID<X>, P>
	{
		void operator()(P *p)
		{
			typedef typename Table<X>::Result T;
			T t;
			t.items.get<X>().value = Singleton<X>::Instance().value;
			unsigned id = Select<T>(p->base).eq<X>(t.items.get<X>().value).Execute();
			if(0 == id)
			{
				Insert_Into<T>(t, p->base).Execute();
				id = Select<T>(p->base).eq<X>(t.items.get<X>().value).Execute();
			}
			p->set<ID<X>>(id);
		}
	};
	template<class P>struct __save__<CurrentPacketTable, P>
	{
		void operator()(P *p){}
	};
}

void PacketBase::Restore()
{
	PacketBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<type_list, SetDefault<type_list> >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		CurrentPacketTable current;
		Select<CurrentPacketTable>(base).ID(1).Execute(current);

		TL::foreach<CurrentPacketTable::items_list, __restore__>()(&current.items, &base);
	}
}

void PacketBase::Save()
{
	PacketBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<PacketBase::type_list, SetDefault<PacketBase::type_list> >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		Update<CurrentPacketTable> update(base);
		TL::foreach<CurrentPacketTable::items_list, __save__>()(&update);
		update.Where().ID(1).Execute();
	}
}

