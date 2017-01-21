#include "stdafx.h"
#include "ItemsData\DataViewer.h"
#include "templates/typelist.hpp"
#include "Filtre\MedianFiltre.h"
#include "App/AppBase.h"
#include "Compute/Compute.h"
#include "DiffApp/App.h"


namespace
{
	static const int count_buffer = 3000;
	struct PTR
	{
		USPC7100_ASCANDATAHEADER *data[count_buffer];
	};
	struct Data
	{
		double data[count_buffer];
	};
}

DefectData::DefectData(int &filterWidth, bool &filterOn, double (&brak)[App::count_zones], double (&klass2)[App::count_zones])
	: medianFiltreWidth(filterWidth)
	, medianFiltreOn(filterOn)
	, brackThreshold(brak) 
	, klass2Threshold(klass2)
	, count(0)
	, zone(0)
{}

void DefectData::Set(int zone_, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s
	, void (*StatusZoneDefect)(int , double , int , double (&)[App::count_zones], double (&)[App::count_zones], char &)
	, USPCViewerData &d
	)
{
	count = 0;
	if(zone_ < 0 || zone_ >= App::count_zones) return;
	zone = zone_;
	stop += offs;
	int cnt = 0;
	if(stop > maxOffs) stop = maxOffs;
	int i = start + offs;
	double _gate1_position = d.param[channel].get<gate1_position>().value;
	double _gate1_width = d.param[channel].get<gate1_width>().value;
	double _scope_offset = d.param[channel].get<scope_offset>().value;

	double mash = 0.001 * s[i].TimeEqu / s[i].DataSize;
	int beg = int((_gate1_position - _scope_offset) / mash);
	int end = int((_gate1_position + _gate1_width - _scope_offset) / mash);
	if(beg < 0 || end < 0)
	{
		beg = int((_gate1_position) / mash);
		end = int((_gate1_position + _gate1_width) / mash);
	}	

	if(!medianFiltreOn)
	{
		for(; i < stop; ++i)
		{
			if(channel == s[i].Channel)
			{
				double t = 0;
				for(int z = beg; z < end; ++z)
				{
					if(s[i].Point[z] > t) t = s[i].Point[z];
				}
				data[cnt] = t;
				scan[cnt] = &s[i];
				(*StatusZoneDefect)(offs, data[cnt], zone, brackThreshold, klass2Threshold, status[cnt]);
				if(d.cancelOperatorSensor[channel][zone]) status[cnt] = StatusId<Clr<Cancel<Projectionist>>>();
				if(++cnt >= dimention_of(data)) break;
			}
		}
	}
	else
	{
		MedianFiltre f;
		int offs = i - medianFiltreWidth * App::count_sensors;
		cnt = -medianFiltreWidth;
		if(offs < 0)
		{
			offs = 0;
			cnt = 0;
		}
	
		f.Clear(medianFiltreWidth);
		for(; i < stop; ++i)
		{
			if(channel == s[i].Channel)
			{
				double t = 0;
				for(int z = beg; z < end; ++z)
				{
					if(s[i].Point[z] > t) t = s[i].Point[z];
				}
				char st;
				(*StatusZoneDefect)(offs, t, zone, brackThreshold, klass2Threshold, st);
				
				int ret = f.Add(t, st, (void *)&s[i]);
				if(cnt >= 0)
				{
					if(StatusId<Clr<DeathZone>>() != st)
					{
						data[cnt] = f.buf[ret];
						scan[cnt] = (USPC7100_ASCANDATAHEADER *)f.data[ret];
						status[cnt] = f.status[ret];
					}
					else
					{
						data[cnt] = t;
						scan[cnt] = &s[i];
						status[cnt] = st;
					}
				}
				if(d.cancelOperatorSensor[channel][zone]) status[cnt] = StatusId<Clr<Cancel<Projectionist>>>();
				if(++cnt >= (int)dimention_of(data)) break;
			}
		}
	}
	count = cnt;
}
