#include "stdafx.h"
#include "ThicknessProtocol.h"
#include "Frame.h"
#include "NetFrame.h"
#include "typelist.hpp"
#include "DebugMess.h"
#include "App.h"
#include "Defectoscope\ItemsData\USPCData.h"

//#pragma pack(1)
struct TcpData
{
	int countZones;
	double data[2][60];
	char   status[60];
	char noop[256];
} tcpData;

namespace 
{
	struct Thick
	{
		TcpData &data;
		Thick()
			: data(tcpData)
		{}
		void Print()
		{
			ItemData<Thickness> &t = Singleton<ItemData<Thickness>>::Instance();
			t.currentOffsetZones = data.countZones;
			for(int i = 0; i < 60; ++i)
			{
				t.buffer[0][i] = data.data[0][i];
				t.buffer[1][i] = data.data[1][i];
				t.status[i] = data.status[i];
				//dprint("%f %f %d\n", t.buffer[0][i], t.buffer[1][i], t.status[i]);
			}
			dprint("\n receive data count items %d\n", t.currentOffsetZones);
			SetEvent(App::WaitThicknessEvent);
		}
	} thick;
}

namespace ThicknessProtocol
{	
	bool Server::Do(Frame *f)
	{
		dprint("Server Ok ################################\n");
		Receive *recvData = (Receive *)&f->data.buf;

		int offs = recvData->from;
		int count = recvData->count;

		char *dta0 = (char *)recvData + sizeof(Receive);
		char *dta1 = &((char *)&thick.data)[offs];
		int size = sizeof(TcpData);
		if(offs + count <= size)
		{
			memmove(dta1, dta0, count);
		}

		if(eLast == recvData->status)
		{
			thick.Print();
			return false;
		}
		else
		{
			Response *r = (Response *)&f->data.buf;
			r->status = eOk;
			r->Next = offs + count;
			f->length = sizeof(Response);
			dprint("server %d\n", r->Next);
		}
		return true;
	}
}