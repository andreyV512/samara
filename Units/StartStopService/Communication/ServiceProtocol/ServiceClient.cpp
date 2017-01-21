#include "stdafx.h"
#include <stdio.h>
#include "ServiceProtocol.h"
#include "Frame.h"
#include "NetClient.h"
#include "typelist.hpp"
#include "IPprotocolProcedureList.hpp"

namespace StatusProtocol
{
	struct Stored: public CommonClient
	{
		Frame *f;
		Stored(Frame *f)
			: f(f)
		{
			f->data.id = TL::IndexOf<__ip_protocol_procedure_list__, StatusProtocol::Server>::value;
			f->length = 0;
			f->proc = Recv<Stored>;
		}

		bool Do()
		{
			printf("%s\n", (char *)f->data.buf);
			f->proc = Stop;
			return true;
		}

		static void *operator new(size_t size, void *ptr)
		{
			return ptr;
		}
		static void operator delete(void *, void *){}
	};		
	void Client::Do(wchar_t *addr, int port)
	{
		Frame *f = Frame::Get();
		new(Frame::AdditionalData<Stored>(f)) Stored(f);
		NetClient::Connect(addr, port, f);
	}
}

namespace RunProtocol
{
	struct Stored: public CommonClient
	{
		Frame *f;
		Stored(Frame *f)
			: f(f)
		{
			f->data.id = TL::IndexOf<__ip_protocol_procedure_list__, RunProtocol::Server>::value;
			f->length = 0;
			f->proc = Recv<Stored>;
		}

		bool Do()
		{
			printf("%s\n", (char *)f->data.buf);
			f->proc = Stop;
			return true;
		}

		static void *operator new(size_t size, void *ptr)
		{
			return ptr;
		}
		static void operator delete(void *, void *){}
	};		
	void Client::Do(wchar_t *addr, int port)
	{
		Frame *f = Frame::Get();
		new(Frame::AdditionalData<Stored>(f)) Stored(f);
		NetClient::Connect(addr, port, f);
	}
}

namespace StopProtocol
{
	struct Stored: public CommonClient
	{
		Frame *f;
		Stored(Frame *f)
			: f(f)
		{
			f->data.id = TL::IndexOf<__ip_protocol_procedure_list__, StopProtocol::Server>::value;
			f->length = 0;
			f->proc = Recv<Stored>;
		}

		bool Do()
		{
			printf("%s\n", (char *)f->data.buf);
			f->proc = Stop;
			return true;
		}

		static void *operator new(size_t size, void *ptr)
		{
			return ptr;
		}
		static void operator delete(void *, void *){}
	};		
	void Client::Do(wchar_t *addr, int port)
	{
		Frame *f = Frame::Get();
		new(Frame::AdditionalData<Stored>(f)) Stored(f);
		NetClient::Connect(addr, port, f);
	}
}