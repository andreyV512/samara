#include "stdafx.h"
#include "Compute.h"
#include "ItemsData/USPCData.h"
#include "Dialogs/Dialogs.h"
#include "Filtre\MedianFiltre.h"
#include "debug_tools/DebugMess.h"
#include "App/AppBase.h"
#include "ItemsData/ResultData.h"
#include "MessageText\SelectMessage.h"
#include "Log\LogMessages.h"
#include "Log\LogBuffer.h"

void StatusZoneThickness(int offs, double &data, int zone, double (&maxThreshold)[App::count_zones]
, double (&minThreshold)[App::count_zones], double (&nominalTreshold)[App::count_zones], char &status)
{
	double min = nominalTreshold[zone] - minThreshold[zone];
	double max = nominalTreshold[zone] + maxThreshold[zone];
	if(0.0 == data)
	{
		status = StatusId<Clr<Undefined>>();
	}
	else if(data < min)
	{
		status = StatusId<Clr<BorderLower<Thickness> >>();
	}
	else  if(data > max)
	{
		status = StatusId<Clr<BorderAbove<Thickness> >>();
	}
	else
	{
		status = StatusId<Clr<Nominal>>();
	}
}

Compute::Compute()
{
}

namespace
{
	struct FiltreOn
	{
		MedianFiltre (&f)[App::count_sensors];
		FiltreOn( MedianFiltre (&f)[App::count_sensors])
			: f(f)
		{}
		inline double operator()(int i, double data)
		{
			MedianFiltre &ff = f[i];
			return ff.buf[ff.Add(data)];
		}
		//inline double operator()(int i, double data, double &bit, int &status, int &status2)
		inline double operator()(int i, double data, int &status)
		{
			MedianFiltre &ff = f[i];
			int index = ff.Add(data, status);
			status = ff.status[index];
			if(StatusId<Clr<BrakStrobe2<Thickness>>>() == status)
			{
				int k = 0;
				for(int z = 0; z < ff.width; ++z)
				{
					if(StatusId<Clr<BrakStrobe2<Thickness>>>() == ff.status[z])	++k;
				}
				if(k > ff.medianIndex) 
				{
					status = StatusId<Clr<BrakStrobe2<Thickness>>>();
				}
			}
			return ff.buf[index];
		}
		inline double AddX(int i, double data)
		{
			MedianFiltre &ff = f[i];
			return ff.bufX[ff.AddX(data)];
		}
	};
	struct FiltreOff
	{
		FiltreOff( MedianFiltre (&f)[App::count_sensors])
		{}
		inline double operator()(int i, double data)
		{
			return data;
		}
		//inline double operator()(int i, double data, double &, int &, int &)
		inline double operator()(int i, double data, int &)
		{
			return data;
		}
		inline double AddX(int i, double data)
		{
			return data;
		}
	};
	template<class T, class Data>void ComputeData(USPCViewerData &d, MedianFiltre (&f)[App::count_sensors]
	, double (&brakThreshold)[App::count_zones], double (&klass2Threshold)[App::count_zones])
	{
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
		T filtre(f);
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			for(int j = 0; j < dimention_of(d.buffer); ++j)
			{
				d.buffer[j][i] = -1;
				d.status[j][i] = StatusId<Clr<Undefined>>();
			}
			for(int jj = d.offsets[i], last = d.offsets[i + 1]; jj < last; ++jj)
			{
				WORD channel = b[jj].Channel;
				if(channel < App::count_sensors)
				{
					//проверить смещение на соответствие каналу
					int j = jj + d.offsSensor[channel];					
					if(j > 0)
					{
						if(channel != b[j].Channel)
						{
							for(int k = j + 1, len = j + App::count_sensors + 2; k < len; ++k)
							{
								if(b[k].Channel == channel)
								{
									j = k;
									break;
								}
							}
						}
						double _gate1_position = d.param[channel].get<gate1_position>().value;
						double _gate1_width = d.param[channel].get<gate1_width>().value;
						double _scope_offset = d.param[channel].get<scope_offset>().value;
						DWORD g1Amp = 0;
						double mash = 0.001 * b[j].TimeEqu / b[j].DataSize;
						int beg = int((_gate1_position - _scope_offset) / mash);
						int end = int((_gate1_position + _gate1_width - _scope_offset) / mash);
						if(beg < 0 || end < 0)
						{
							beg = int((_gate1_position) / mash);
							end = int((_gate1_position + _gate1_width) / mash);
						}	
						for(int z = beg; z < end; ++z)
						{
							if(b[j].Point[z] > g1Amp) g1Amp = b[j].Point[z];
						}
						double t = filtre(channel, g1Amp);
						int z = jj / App::count_sensors;
						z *= App::count_sensors;
						if(z < d.deadZoneSamplesBeg || z > d.deadZoneSamplesEnd)
						{
						//	if(StatusId<Clr<Undefined>>() == d.status[channel][i])
							{
								d.status[channel][i] = StatusId<Clr<DeathZone>>();
							}
						}
						else
						{							
							if(t > d.buffer[channel][i])
							{
								d.buffer[channel][i] = t;						
								StatusZoneDefect<Data>(j, t, i, brakThreshold, klass2Threshold, d.status[channel][i]);
							}
							if(d.cancelOperatorSensor[channel][i]) d.status[channel][i] = StatusId<Clr<Cancel<Projectionist>>>();
						}
					}	
				}
			}
		}
		int buf[ App::count_sensors + 1];
		buf[App::count_sensors] = -1;
		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			for(int j = 0; j < App::count_sensors; ++j)
			{
				buf[j] = d.status[j][i];
			}
			int t = 0;
			SelectMessage(buf, t);
			d.commonStatus[i] = t;
		}
	}

	template<class T, class Data>void ComputeData(USPCViewerThicknessData &d, MedianFiltre (&f)[App::count_sensors]
	, double (&normThickness)[App::count_zones], double (&minThickness)[App::count_zones], double (&maxThickness)[App::count_zones])
	{
		unsigned start = GetTickCount();
		USPC7100_ASCANDATAHEADER *b = d.ascanBuffer;
		
		T filtre(f);
		double brackStrobe = Singleton<BrackStrobe2Table>::Instance().items.get< BrakStrobe2<Thickness>>().value;
		static const int buf_size = 512;
		double data[App::count_sensors][buf_size];
		char status[App::count_sensors][buf_size];


		for(int i = 0; i < d.currentOffsetZones; ++i)
		{
			double nominal = maxThickness[i];
			d.bufferMin[i] = 1000;
			d.bufferMax[i] = -1;
			d.commonStatus[i] =  StatusId<Clr<Undefined>>();
			ItemData<Thickness> &uspc = Singleton<ItemData<Thickness>>::Instance();
			int offset = 0;
			for(int jj = d.offsets[i], last = d.offsets[i + 1]; jj < last; ++jj)
			{
				WORD channel = b[jj].Channel;	
				if(channel < App::count_sensors)
				{
					//проверить смещение на соответствие каналу
					int j = jj + d.offsSensor[channel];					
					if(j > 0)
					{
						if(channel != b[j].Channel)
						{
							for(int k = j + 1, len = j + App::count_sensors + 2; k < len; ++k)
							{
								if(b[k].Channel == channel)
								{
									j = k;
									break;
								}
							}
						}
						double t = 999999;
						static const int Status = StatusId<Clr<BrakStrobe2<Thickness>>>();
						int st = StatusId<Clr<Undefined>>();
						if(b[j].hdr.G1Tof)
						{
							double gate1_position_ = uspc.param[b[j].Channel].get<gate1_position>().value;
							double gate1_width_ = uspc.param[b[j].Channel].get<gate1_width>().value;
							double strob = 0.005 * b[j].hdr.G1Tof;
							if(gate1_position_ < strob && (gate1_position_ + gate1_width_) >  strob)
							{
								t = 2.5e-6 * b[j].hdr.G1Tof * d.param[channel].get<gate1_TOF_WT_velocity>().value;
								if(b[j].hdr.G2Tof)
								{
									double gate2_position_ = uspc.param[b[j].Channel].get<gate2_position>().value;
									double gate2_width_ = uspc.param[b[j].Channel].get<gate2_width>().value;
									double strob = 0.005 * b[j].hdr.G2Tof;
									if(gate2_position_ < strob && (gate2_position_ + gate2_width_) >  strob)
									{
										double val2 = 2.5e-6 * b[j].hdr.G2Tof * d.param[channel].get<gate2_TOF_WT_velocity>().value;
										double tt = t - val2;
										if(tt > brackStrobe)
										{
											st = Status;
										}
									}
								}
							}
						}

						int offs = offset / App::count_sensors;
						
						t = filtre(channel, t, st);
						status[channel][offs] = st;
						data[channel][offs] = t;

						int z = jj / App::count_sensors;
						z *= App::count_sensors;
						if(z < d.deadZoneSamplesBeg || z > d.deadZoneSamplesEnd)
						{
							status[channel][offs] = StatusId<Clr<DeathZone>>();
						}
						else if(999999 != t)
						{
							char stat =  StatusId<Clr<Undefined>>();
							StatusZoneThickness(j, t, i, normThickness, minThickness, maxThickness, stat);
							if(StatusId<Clr<Undefined>>() != st)
							{
								int x[] = {
									stat
									, st
									, -1
								};
								int res = 0;
								SelectMessage(x, res);
								status[channel][offs] = res;
							}
							else
							{
								status[channel][offs] = stat;
							}
						}
						
					}
				}	
				if(++offset >= buf_size * App::count_sensors)break;
			}
			//--------------------------------------------------------------
			int countSamples = offset / App::count_sensors;
			double sum[App::count_sensors] = {};
			int count[App::count_sensors] = {};
			for(int k = 0; k < App::count_sensors; ++k)
			{
				for(int j = 0; j <  countSamples; ++j)
				{
					char s = status[k][j]; 
					if(StatusId<Clr<Undefined>>() != s)
					{
						sum[k] += data[k][j];
						++count[k];
					}
				}
			}
			for(int k = 0; k < App::count_sensors; ++k)
			{
				if(0 == count[k]) count[k] = 1;
				double tsum = sum[k] / count[k];
				for(int j = 0; j <  countSamples; ++j)
				{
					char s = status[k][j];
					
					if(StatusId<Clr<Undefined>>() == s)
					{
						data[k][j] = tsum;
					}
					else 
					{
						if(1.5 * tsum < data[k][j])
						{
							data[k][j] = tsum;
							status[k][j] = StatusId<Clr<Undefined>>();
						}
					}
					
				}
			}
			double min = 999999;
			double max = 0;
			char st[App::count_sensors] = {};
			for(int k = 0; k < App::count_sensors; ++k)
			{	
				if(!d.cancelOperatorSensor[k][i])
				{
					char stat = StatusId<Clr<Undefined>>();
					for(int j = 0; j <  countSamples; ++j)
					{
						char s = status[k][j];
						if(!IsBrackStrobe(s) && StatusId<Clr<Undefined>>() != s)
						{
							double t = data[k][j];
							if(min > t) min = t;
							else if(max < t) max = t;
						}
						
						int res;
						if(StatusId<Clr<Undefined>>() != s)
						{
							int x[] = {
								s
								, st[k]
								, -1
							};
							SelectMessage(x, res);
							st[k] = res;
						}
					}
				}

			}
			d.bufferMin[i] = min;
			d.bufferMax[i] = max;
//--------------------------------------------------------------
#define xxx(n) d.cancelOperatorSensor[n][i] ? StatusId<Clr<Cancel<Projectionist>>>(): st[n]

			int x[] =
			{
				xxx(0)
				, xxx(1)
				, xxx(2)
				, xxx(3)
				, xxx(4)
				, xxx(5)
				, xxx(6)
				, xxx(7)
				, -1
			};
			int res = 0;
			SelectMessage(x, res);
			d.commonStatus[i] = res;
#undef xxx	
//¬ычисление скольз€щего среднего
			double average[App::count_sensors] = {};
			for(int k = 0; k < App::count_sensors; ++k)
			{				
				if(!d.cancelOperatorSensor[k][i])
				{
					double t = 0;
					int count = 0;
					for(int j = 0; j < countSamples; ++j)
					{
						if(StatusId<Clr<Nominal>>() == status[k][j])
						{
							t += data[k][j];
							++count;
						}						
					}
					if(count)
					{
						average[k] = t / count;
					}
				}
			}
			min = 999999;
			max = 0;
			for(int k = 0; k < App::count_sensors; ++k)
			{
				double t = average[k];
				if(t)
				{
					if(t > max) max = t;
					if(t < min) min = t;
				}
			}
			d.movingAverageMax[i] = max;
			d.movingAverageMin[i] = 999999 != min ? min : 0;
		}
	}

	void UndefinedItem(USPCViewerData &x)
	{
		ZeroMemory(x.status, sizeof(x.status));
		ZeroMemory(x.buffer, sizeof(x.buffer));
		x.currentOffsetZones = 0;
	}

	void UndefinedItem(USPCViewerThicknessData &x)
	{
		ZeroMemory(x.bufferMax, sizeof(x.bufferMax));
		ZeroMemory(x.bufferMin, sizeof(x.bufferMin));
		ZeroMemory(x.commonStatus, sizeof(x.commonStatus));
		x.currentOffsetZones = 0;
	}

	template<class O, class P>struct __recalculation__
	{
		void operator()()
		{
			ItemData<O> &data = Singleton<ItemData<O> >::Instance();
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
			{
				data.SamplesPerZone(compute.lengthTube
					, Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0<O>>().value
					, Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1<O>>().value
					);
				MedianFiltre f[App::count_sensors];
				if(Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<O> >().value)
				{
					int width = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<O> >().value;
					width |= 1;
					for(int i = 0; i < dimention_of(f); ++i) f[i].Clear(width);
					ComputeData<FiltreOn, O>(data
						, f
						, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<O> >().value
						);
				}
				else
				{
					ComputeData<FiltreOff, O>(data
						, f
						, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<O> >().value
						);
				}
			}
			else
			{
				UndefinedItem(data);
			}
		}
	};

	template<class P>struct __recalculation__<Thickness, P>
	{
		typedef Thickness O;
		void operator()()
		{
			ItemData<O> &data = Singleton<ItemData<O> >::Instance();
			if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
			{
				data.SamplesPerZone(compute.lengthTube
					, Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0<O>>().value
					, Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1<O>>().value
					);
				MedianFiltre f[App::count_sensors];
				if(Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<O> >().value)
				{
					int width = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<O> >().value;
					width |= 1;
					for(int i = 0; i < dimention_of(f); ++i) f[i].Clear(width);
					ComputeData<FiltreOn, O>(Singleton<ItemData<O> >::Instance()
						, f
						, Singleton<ThresholdsTable>::Instance().items.get<BorderAbove<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderLower<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<O> >().value
						);
				}
				else
				{
					ComputeData<FiltreOff, O>(Singleton<ItemData<O> >::Instance()
						, f
						, Singleton<ThresholdsTable>::Instance().items.get<BorderAbove<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderLower<O> >().value
						, Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<O> >().value
						);
				}
			}
			else
			{
				UndefinedItem(data);
			}
		}
	};	

	void CommonStatus(bool &tubeResult)
	{
		bool crossOnJob     = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Cross> >().value;
		bool longOnJob      = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long> >().value;
		bool thicknessOnJob = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thickness> >().value;

		char (&crossStatus)[App::count_zones] = Singleton<ItemData<Cross> >::Instance().commonStatus;
		char (&longStatus)[App::count_zones] = Singleton<ItemData<Long> >::Instance().commonStatus;
		char (&thicknessStatus)[App::count_zones] = Singleton<ItemData<Thickness> >::Instance().commonStatus;

		char (&resultStatus)[App::count_zones] = Singleton<ResultViewerData>::Instance().commonStatus;
		int &currentOffset = Singleton<ResultViewerData>::Instance().currentOffsetZones;

		ZeroMemory(resultStatus, sizeof(currentOffset));

		currentOffset = 0;

		if(crossOnJob) if(currentOffset < Singleton<ItemData<Cross> >::Instance().currentOffsetZones) currentOffset = Singleton<ItemData<Cross> >::Instance().currentOffsetZones;
		if(longOnJob) if(currentOffset < Singleton<ItemData<Long> >::Instance().currentOffsetZones) currentOffset = Singleton<ItemData<Long> >::Instance().currentOffsetZones;
		if(thicknessOnJob) if(currentOffset < Singleton<ItemData<Thickness> >::Instance().currentOffsetZones) currentOffset = Singleton<ItemData<Thickness> >::Instance().currentOffsetZones;

		int buf[4];

		for(int i = 0; i < currentOffset; ++i)
		{
			int k = 0;
			memset(buf, -1, sizeof(buf));

			if(crossOnJob    ) buf[k++] = crossStatus[i]    ;
			if(longOnJob     ) buf[k++] = longStatus[i]     ;
			if(thicknessOnJob) buf[k  ] = thicknessStatus[i];

			int t = 0;

			SelectMessage(buf, t);

			resultStatus[i] = t;
		}
		tubeResult = TubeResult(resultStatus, currentOffset);
	}

}

template<class O, class P>struct __collection_data_ok__
{
	bool operator()()
	{
		if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
		{
			if(0 == Singleton<ItemData<O> >::Instance().currentOffsetFrames)
			{
				Log::Mess<LogMess::AlarmNoDataCollection>();
				return false;
			}
		}
		return true;
	}
};

template<class O, class P>struct __buffer_over_flow__
{
	bool operator()()
	{
		if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
		{
			int count = Singleton<ItemData<O> >::Instance().currentOffsetFrames;
			zprint("overflow %d  %d\n", App::count_frames, count);
			if(App::count_frames > count) return true;
			Log::Mess<LogMess::AlarmBufferOverflow>();
			return false;
		}
		return true;
	}
};

template<class O, class P>struct __min_size__
{
	void operator()(P &p)
	{
		if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
		{
			if(0 == p)
			{
				p = Singleton<ItemData<O> >::Instance().currentOffsetFrames;
			}
			else if(p > Singleton<ItemData<O> >::Instance().currentOffsetFrames)
			{
				p = Singleton<ItemData<O> >::Instance().currentOffsetFrames;
			}
		}
	}
};

template<class O, class P>struct __set_size__
{
	void operator()(P &p)
	{
		if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<O> >().value)
		{
			Singleton<ItemData<O> >::Instance().currentOffsetFrames = p;
		}
	}
};

void Compute::Recalculation()
{	
	bool dataOk = TL::find<USPC::items_list, __collection_data_ok__>()();//проверка на наличие данных в буфере
	bool bufferNotOverflow = TL::find<USPC::items_list, __buffer_over_flow__>()();
	if(!bufferNotOverflow) return; //ѕроверка на переполнение буфера
	int minSize = 0;
	TL::foreach<USPC::items_list, __min_size__>()(minSize);
	minSize /= App::count_sensors;
	minSize *= App::count_sensors;
	TL::foreach<USPC::items_list, __set_size__>()(minSize);
	TL::foreach<USPC::items_list, __recalculation__>()();
	CommonStatus(tubeResult);
	if(dataOk)
	{		
		double len = 0.001 * lengthTube;
		if(tubeResult)
		{
			Log::Mess<LogMess::CycleOk>(len);
		}
		else
		{
			Log::Mess<LogMess::CycleBrak>(len);
		}
	}
	app.MainWindowUpdate();
}

void Compute::LengthTube(unsigned startTime, unsigned baseTime, unsigned stopTime)
{
	double offs = Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset1>().value; 
	lengthTube = int(offs * (stopTime - startTime) /(baseTime - startTime));
	lengthTube -= App::lengthCaretka;
}

Compute compute;

void RecalculationDlg::Do(HWND)
{
	compute.Recalculation();
}
namespace
{
	template<class O, class P>struct __clear__
	{
		void operator()()
		{
			ItemData<O> &o = Singleton<ItemData<O>>::Instance();
			memset(o.cancelOperatorSensor, 0, sizeof(o.cancelOperatorSensor));
		}
	};
}
void Compute::CancelOperatorClear()
{
	TL::foreach<USPC::items_list, __clear__>()();
}
