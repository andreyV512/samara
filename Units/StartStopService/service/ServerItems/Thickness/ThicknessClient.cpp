#include "stdafx.h"
#include "ThicknessProtocol.h"
#include "ServerHandler.h"
#include "Frame.h"
#include "NetClient.h"
#include "CommonTCP.h"
#include "IPprotocolProcedureList.hpp"
#include "typelist.hpp"
#include "DebugMess.h"

namespace 
{
	struct ThickClient
	{
		int countZones;
		double data0[360];
		double data1[360];
		char   status[360];
		ThickClient()
		{
			for (int i = 0; i < 360; ++i)
			{
				  data0[i] = i * 10;
				  data1[i] = i * 10  + 1;
				  status[i] = i;
			}
			countZones = 360;
		}
	} thickClient;
}

namespace ThicknessProtocol
{
	struct Stored: public CommonClient
	{
		int to;
		ThickClient &item;
		Frame *f;
		Stored(ThickClient &item, Frame *f, int from_, int to_)
			: item(item)
			, f(f)
			, to(to_)
		{
			if(to > item.countZones) to = item.countZones;
			Response *res = (Response *)f->data.buf;
			res->count = (sizeof(f->data.buf) - sizeof(Receive)) / (2 * sizeof(double) + sizeof(char));
			res->Next = from_;
			f->data.id = TL::IndexOf<__ip_protocol_procedure_list__, ThicknessProtocol::Server>::value; 
		}
		
		bool Do()
		{
			Response res = *(Response *)f->data.buf;
			Receive *r = (Receive *)f->data.buf;
			if(res.Next + res.count < to)
			{
				r->from = res.Next;
				r->count = res.count;
				f->proc = Recv<Stored>;
				r->status = eOk;
			}
			else if(res.Next < to)
			{
				r->from = res.Next;
				r->count = to - res.Next; 
				f->proc = Stop;
				r->status = eLast;
			}
			else
			{
				return false;
			}

			char *ofs = f->data.buf + sizeof(Receive);
			double *d = (double *)ofs;
			char *c = (char *)&d[r->count];
			unsigned halfBuf = r->count * sizeof(double) / 2;
			memmove(d, &item.data0[r->from / 2], halfBuf);
			memmove(&d[r->count / 2], &item.data1[r->from/ 2], halfBuf);
			memmove(c, &item.status[r->from], r->count * sizeof(char) / 2);
			f->length = 2 * halfBuf +  r->count * sizeof(char) / 2;

			dprint("%d    %d\n", res.Next + res.count, to);
			return true;
		}
		static void *operator new(size_t size, void *ptr)
		{
			 return ptr;
		}
		static void operator delete(void *, void *){}
	};
	
	void Client::Do(wchar_t *addr, int port, int from, int count)
	{
	    Frame *f = Frame::Get();

		Stored *s = new(Frame::AdditionalData<Stored>(f)) Stored(thickClient, f, from, count);

		if(s->Do())
		{
			dprint("start client \n");
			NetClient::Connect(addr, port, f);
		}
		else
		{
			Frame::Drop(&f);
		}
	}
}

