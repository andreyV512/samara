#pragma once
#include "DiffApp/App.h"
#include "App/AppBase.h"
#include "debug_tools/DebugMess.h"
#include "templates/typelist.hpp"
#include "ItemsData/USPCData.h"
#include "Compute/Compute.h"

struct DataVieverConst
{
	static const int buf_size = 2 * 2048;
};

struct DefectData
{
	double data[DataVieverConst::buf_size];
	USPC7100_ASCANDATAHEADER *scan[DataVieverConst::buf_size];
	char status[DataVieverConst::buf_size];
	int count;
	int zone;
	int &medianFiltreWidth;
	bool &medianFiltreOn;
	double (&brackThreshold )[App::count_zones];
	double (&klass2Threshold)[App::count_zones];
	DefectData(int &, bool &, double (&)[App::count_zones], double (&)[App::count_zones]);
	void Set(int zone, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s
		, void (*)(int , double , int , double (&)[App::count_zones], double (&)[App::count_zones], char &)
		,  USPCViewerData &);
};

class OffsetChannel
{
	struct __data__
	{
		int channel, offset;
	};
	template<class O, class P>struct __offs__
	{
		bool operator()(O *o, P *p)
		{
			if(O::ID == p->channel)
			{
				p->offset = o->value;
				return false;
			}
			return true;
		}
	};
public:
	int operator()(int channel)
	{
		__data__ data = {channel, 0};
		TL::find<OffsetsTable::items_list, __offs__>()(&Singleton<OffsetsTable>::Instance().items, &data);
		return data.offset;
	}
};

template<class T>struct DataViewer: DefectData
{
	DataViewer()
		: DefectData(
		   Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T> >().value
		   , Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T> >().value
		)
	{}
	void Do(int zone, int channel)
	{
		memset(scan, 0, sizeof(scan));
		ItemData<T> &d = Singleton<ItemData<T> >::Instance();
		int start = d.offsets[zone];
		int stop = d.offsets[1 + zone];
		int offs = 0;//OffsetChannel()(channel);
		int maxOffs = d.currentOffsetFrames;
		USPC7100_ASCANDATAHEADER *s = d.ascanBuffer;

		start += d.offsSensor[channel];//samplesOffset;
		if(start < 0) start = 0;
		stop += d.offsSensor[channel];//samplesOffset;
		if(stop < 0) start = stop = 0;
		Set(zone, start, stop, channel, offs, maxOffs, s, StatusZoneDefect<T>
			, d
			);
	}
};

struct ThicknessData
{
	double data[DataVieverConst::buf_size];
	USPC7100_ASCANDATAHEADER *scan[DataVieverConst::buf_size];
	char status[DataVieverConst::buf_size];
	int count;
	int zone;
	int &medianFiltreWidth;
	bool &medianFiltreOn;
	double (&aboveBorder  )[App::count_zones];
	double (&lowerBorder  )[App::count_zones];
	double (&nominalBorder)[App::count_zones];
	ThicknessData(int &, bool &, double (&)[App::count_zones], double (&)[App::count_zones], double (&)[App::count_zones]);
	void Set(int zone, int start, int stop, int channel, int offs, int maxOffs, USPC7100_ASCANDATAHEADER *s);
};
template<>struct DataViewer<Thickness>: ThicknessData
{
	typedef Thickness T;
	DataViewer()
		: ThicknessData(
		   Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T> >().value
		   , Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderAbove  <T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderLower  <T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<T> >().value
		)
	{}
	void Do(int zone, int channel)
	{
		memset(scan, 0, sizeof(scan));
		ItemData<T> &d = Singleton<ItemData<T> >::Instance();
		int start = d.offsets[zone];
		int stop = d.offsets[1 + zone];
		int offs = 0;
		int maxOffs = d.currentOffsetFrames;
		USPC7100_ASCANDATAHEADER *s = d.ascanBuffer;

		start += d.offsSensor[channel];
		if(start < 0) start = 0;
		stop += d.offsSensor[channel];
		if(stop < 0) start = stop = 0;
		Set(zone, start, stop, channel, offs, maxOffs, s);
	}
};