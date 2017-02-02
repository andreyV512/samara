#include "stdafx.h"
#include "templates/typelist.hpp"
#include "Filtre\MedianFiltre.h"
#include "App/AppBase.h"
#include "Compute/Compute.h"
#include "ItemsData\DataViewer.h"
#include "MessageText\SelectMessage.h"

ThicknessData::ThicknessData(int &filterWidth, bool &filterOn
	, double (&aboveBorder)[App::count_zones]
, double (&lowerBorder)[App::count_zones]
, double (&nominalBorder)[App::count_zones]
)
	: medianFiltreWidth(filterWidth)
	, medianFiltreOn(filterOn)
	, aboveBorder  (aboveBorder)
	, lowerBorder  (lowerBorder)
	, nominalBorder(nominalBorder)
	, count(0)
	, zone(0)
{}

void ThicknessData::Set(int zone_, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s)
{
	count = 0;
	if(zone_ < 0 || zone_ >= App::count_zones) return;
	zone = zone_;
	stop += offs;
	int cnt = 0;
	if(stop > maxOffs) stop = maxOffs;
	int i = start + offs;
	ItemData<Thickness> &d = Singleton<ItemData<Thickness> >::Instance();
	double brackStrobe = Singleton<BrackStrobe2Table>::Instance().items.get<BrakStrobe2<Thickness>>().value;
	int ret = 0;
	if(!medianFiltreOn)
	{		
		for(; i < stop; ++i)
		{
			if(channel == s[i].Channel)
			{
				static const int Status = TL::IndexOf<ColorTable::items_list, Clr<BrakStrobe2<Thickness>>>::value;
				char st = StatusId<Clr<Undefined>>();
				data[cnt] = 0;
				if(s[i].hdr.G1Tof)
				{
					double gate1_position_ = d.param[channel].get<gate1_position>().value;
					double gate1_width_ = d.param[channel].get<gate1_width>().value;
					double strob = 0.005 * s[i].hdr.G1Tof;
					if(gate1_position_ < strob && (gate1_position_ + gate1_width_) >  strob)
					{
						double val = 2.5e-6 * s[i].hdr.G1Tof * d.param[channel].get<gate1_TOF_WT_velocity>().value;
						data[cnt] = val;
						if(s[i].hdr.G2Tof)
						{
							double val2 = 2.5e-6 * s[i].hdr.G2Tof * d.param[channel].get<gate2_TOF_WT_velocity>().value;
							double t = val - val2;
							if(t > brackStrobe)
							{
								st = Status;
							}
						}
					}
				}
				scan[cnt] = &s[i];
				if(Status == st)
				{
					status[cnt] = st;
				}
				else
				{					
					StatusZoneThickness(offs, data[cnt], zone
						, aboveBorder  
						, lowerBorder  
						, nominalBorder
						, status[cnt]);
				}
				if(d.cancelOperatorSensor[channel][zone]) status[cnt] = StatusId<Clr<Cancel<Projectionist>>>();
				if( StatusId<Clr<Undefined>>() == status[cnt]) 
				  data[cnt] = Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<Thickness> >().value[zone];
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
				double t = 999999;
				static const int Status = StatusId<Clr<BrakStrobe2<Thickness>>>();
				char st = StatusId<Clr<Undefined>>();
				if(s[i].hdr.G1Tof)
				{
					double gate1_position_ = d.param[channel].get<gate1_position>().value;
					double gate1_width_ = d.param[channel].get<gate1_width>().value;
					double strob = 0.005 * s[i].hdr.G1Tof;
					if(gate1_position_ < strob && (gate1_position_ + gate1_width_) >  strob)
					{
						t = 2.5e-6 * s[i].hdr.G1Tof *d.param[channel].get<gate1_TOF_WT_velocity>().value;
						if(s[i].hdr.G2Tof)
						{
							double gate2_position_ = d.param[channel].get<gate2_position>().value;
							double gate2_width_ = d.param[channel].get<gate2_width>().value;
							double strob = 0.005 * s[i].hdr.G2Tof;
							if(gate2_position_ < strob && (gate2_position_ + gate2_width_) >  strob)
							{
								double val2 = 2.5e-6 * s[i].hdr.G2Tof * d.param[channel].get<gate2_TOF_WT_velocity>().value;
								double tt = t - val2;
								if(tt > brackStrobe)
								{
									st = Status;
								}
							}
						}
					}
				}
				status[cnt] = StatusId<Clr<Undefined>>();				
				{
					ret = f.Add(t, st, (void *)&s[i]);
					t = f.buf[ret];
					st = f.status[ret];
					if(Status == st)
					{
						int k = 0;
						for(int z = 0; z < f.width; ++z)
						{
							if(Status == f.status[z])	++k;
						}
						if(k > f.medianIndex) st = Status;
					}
					if(cnt >= 0)
					{
						scan[cnt] = (USPC7100_ASCANDATAHEADER *)f.data[ret];
						status[cnt] = st;
						data[cnt] = t;
					}
				}
				
				if(cnt >= 0)
				{
					if(999999 != t)
					{
						char stat = StatusId<Clr<Undefined>>();
						StatusZoneThickness(offs, t, zone
							, aboveBorder  
							, lowerBorder  
							, nominalBorder
							, stat
							);
						if(StatusId<Clr<Undefined>>() != status[cnt])
						{
							int x[] = {
								stat
								, status[cnt]
								, -1
							};
							int res = 0;
							SelectMessage(x, res);
							status[cnt] = res;
						}
						else 
						{
							status[cnt] = stat;
						}
					}
					else
					{
						data[cnt] = 0;
					}
					if(d.cancelOperatorSensor[channel][zone]) status[cnt] = StatusId<Clr<Cancel<Projectionist>>>();
				}
				if(++cnt >= (int)dimention_of(data)) break;
			}			
		}
		double sum = 0;
		int count =0;
		for(int i = 0; i < cnt; ++i)
		{
			if(/*!IsBrackStrobe(status[i]) && */StatusId<Clr<Undefined>>() != status[i])
			{
				sum += data[i];
				++count;
			}
		}
		if(0 == count) count = 1;
		sum /= count;
		for(int i = 0; i < cnt; ++i)
		{			
			if(StatusId<Clr<Undefined>>() == status[i])
			{
				data[i] = sum;
			}
			else //if(IsBrackStrobe(status[i]))
			{
				if(1.5 * sum < data[i])
				{
					data[i] = sum;
					status[i] = StatusId<Clr<Undefined>>();
				}
			}
		}
	}
	count = cnt;
}
