#include "stdafx.h"
#include "ItemsData/USPCData.h"
#include "ItemsData/ConstData.h"
#include "App/AppBase.h"
#include <math.h>
#include <Mmsystem.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "debug_tools/DebugMess.h"
#include "USPC/ut_files.h"
#include "Ini\ItemIni.h"

#pragma comment(lib, "Winmm.lib")

void USPCData::Start()
{
	currentOffsetFrames = 0;	
}

UCHAR *USPCData::CurrentFrame()
{
	return (UCHAR *)&ascanBuffer[currentOffsetFrames];
}
void USPCData::OffsetCounter(int offs)
{
	currentOffsetFrames += offs;
}

namespace
{
	template<class O, class P>struct __sensors_offset_in_samples__
	{
		void operator()(O *o, P *p)
		{
			static const int i = TL::IndexOf<OffsetsTable::items_list, O>::value;
			double t = (double)p->samplesPerZone * o->value / App::zone_length;// / App::count_sensors;
			p->offsSensor[i] = (int)t;
			p->offsSensor[i] /= App::count_sensors;
			p->offsSensor[i] *= App::count_sensors;
		}
	};
}

void USPCData::SamplesPerZone(int tubeLength, int deadArea0, int deadArea1)
{
	samplesPerZone = (double)App::zone_length * currentOffsetFrames 
		/ (tubeLength + App::lengthCaretka);
	ZeroMemory(offsets, sizeof(offsets));
	for(int i = 0; i < App::count_zones; ++i)
	{
	   offsets[i] = int(samplesPerZone * i);
	}
	//смещение в отчётах датчиков на каретке
	TL::foreach<OffsetsTable::items_list, __sensors_offset_in_samples__>()(&Singleton<OffsetsTable>::Instance().items, this);
	currentOffsetZones = int((double)(tubeLength) / App::zone_length);
	int lastZoneSize = tubeLength - currentOffsetZones * App::zone_length;
	if(lastZoneSize > App::zone_length / 3)  ++currentOffsetZones;
	//число отчётов в мёртвой зоне начало
	double t = deadArea0;
	t *= samplesPerZone;
	t /=  App::zone_length;
    deadZoneSamplesBeg  = (int)t;
	deadZoneSamplesBeg /= App::count_sensors;
	deadZoneSamplesBeg *= App::count_sensors;
	//число отчётов в мёртвой зоне конец
	t = tubeLength - deadArea1;
	t *= samplesPerZone;
	t /=  App::zone_length;
    deadZoneSamplesEnd  = (int)t;

	deadZoneSamplesEnd /= App::count_sensors;
	--deadZoneSamplesEnd;
	deadZoneSamplesEnd *= App::count_sensors;

	for(int i = 0; i < App::count_zones; ++i)
	{
		offsets[i] /= App::count_sensors;
		offsets[i] *= App::count_sensors;
	}
}
//-----------------------------------------------------------------------------------------
