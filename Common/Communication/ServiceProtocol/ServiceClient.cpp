#include "stdafx.h"
#include <stdio.h>
#include "ServiceProtocol.h"
#include "ProtocolTCP/Frame.h"
#include "ProtocolTCP/NetClient.h"
#include "templates/typelist.hpp"
#include "..\IPprotocolProcedureList.hpp"
#include "debug_tools/DebugMess.h"

#pragma warning(disable : 4996)

namespace StatusProtocol
{
	struct Stored: public CommonClient
	{
		Frame *f;
		HANDLE h;
		char *status;
		Stored(Frame *f, char *s, HANDLE h)
			: f(f)
			, status(s)
			, h(h)
		{
			f->data.id = TL::IndexOf<__ip_protocol_procedure_list__, StatusProtocol::Server>::value;
			f->length = 0;
			f->proc = Recv<Stored>;
			f->data.buf[0] = '\0';
		}

		bool Do()
		{
			strcpy(status, f->data.buf); 
			dprint("%s\n", f->data.buf);
			SetEvent(h);
			f->proc = Stop;		
			return true;
		}

		static void *operator new(size_t size, void *ptr)
		{
			return ptr;
		}
		static void operator delete(void *, void *){}
	};		
	bool Client::Do(wchar_t *addr, int port, char *status)
	{
		Frame *f = Frame::Get();
		HANDLE h = CreateEvent(NULL, FALSE, FALSE, NULL);
		new(Frame::AdditionalData<Stored>(f)) Stored(f, status, h);
		NetClient::Connect(addr, port, f);
		return WAIT_TIMEOUT == WaitForSingleObject(h, 60000);
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
			f->data.buf[0] = '\0';
		}

		bool Do()
		{
			dprint("%s\n", f->data.buf);
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
			f->data.buf[0] = '\0';
		}

		bool Do()
		{
			printf("%s\n", f->data.buf);
			f->proc = Stop;
			return true;
		}

		static void *operator new(size_t size, void *ptr)
		{
			return ptr;
		}
		static void operator delete(void *, void *){}
	};		
	bool Client::Do(wchar_t *addr, int port)
	{
		if(0 == *addr) return false;
		Frame *f = Frame::Get();
		new(Frame::AdditionalData<Stored>(f)) Stored(f);
		return NetClient::Connect(addr, port, f);
	}
}