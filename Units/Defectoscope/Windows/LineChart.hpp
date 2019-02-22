#pragma once
#include "App/AppBase.h"
#include "ScanWindow.h"
#include "Ini/ItemIni.h"
#include "templates/templates.hpp"
#include "window_tool\MenuAPI.h"

namespace
{
	template<class T>struct Title;
	template<>struct Title<Cross>{wchar_t *operator()(){return L"Поперечный";}};
	template<>struct Title<Long>{wchar_t *operator()(){return L"Продольный";}};
	template<>struct Title<Thickness>{wchar_t *operator()(){return L"Толщина";}};
};

template<class T>struct DefVal
{
	double operator()(double d, int){return d;};
};

template<>struct DefVal<Thickness>
{
	double operator()(double d, int zone)
	{
		if(0.0 == d)
		{
			return Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<Thickness> >().value[zone];
		}
		return d;
	}
};

template<class T, int N>struct Line: LineTresholdsViewer<typename TL::SelectT<ThresholdsTable::items_list, typename T::sub_type>::Result>
{
	typedef LineTresholdsViewer<typename TL::SelectT<ThresholdsTable::items_list, typename T::sub_type>::Result> Parent;
	T *owner;		
	DataViewer<typename T::sub_type> dataViewer;
	Line()
	{
		((Parent::TChart *)chart)->items.get<BarSeries>().SetColorBarHandler(this, &Line::GetColorBar);
		cursor->SetMouseMoveHandler(this, &Line::CursorDraw);
	}	
	bool GetColorBar(int offs, double &data, unsigned &color)
	{
		if(NULL != dataViewer.data && offs < dataViewer.count)
		{
			data = DefVal<typename T::sub_type>()(dataViewer.data[offs], offs);
			color = ConstData::ZoneColor(dataViewer.status[offs]);
			return true;
		}
		return false;
	}

	bool CursorDraw(TMouseMove &l, VGraphics &g)	  
	{	
		if(owner->drawZones)
		{
			double dX = (chart->rect.right - chart->rect.left - chart->offsetAxesLeft - chart->offsetAxesRight)
				/(chart->maxAxesX - chart->minAxesX);
			storedMouseMove.x = (WORD)(chart->rect.left + chart->offsetAxesLeft + dX * offsetX + dX / 2);

			double valY = ((Parent::TChart *)chart)->items.get<BarSeries>().ValueY(offsetX);
			int color;
			bool b;
			char *s = StatusText()(dataViewer.status[offsetX], color, b);
			if(TL::IndexOf<ColorTable::items_list, Clr<Undefined>>::value != dataViewer.status[offsetX])
			{
				char val[128];
				sprintf(val, "%.2f", valY);
				for(char *ss = val; *ss; ++ss){if('.' == *ss){ss += 2;*ss = '\0';break;}}
				wsprintf(label.buffer, L"<ff>Зона <ff0000>%d <ff>датчик <ff0000>%d <ff>смещение %d  величина %S   %S     "
					, 1 + owner->lastZone, 1 + N, 1 + offsetX, val, s);
			}
			else
			{
				wsprintf(label.buffer, L"<ff>Зона <ff0000>%d <ff>датчик <ff0000>%d <ff>смещение %d   %S     "
					, 1 + owner->lastZone, 1 + N, 1 + offsetX, s);
			}
		}
		else
		{
			label.buffer[0] = 0;
		}
		label.Draw(g());
		return true;
	}

	void MenuItemScan()
	{
		Scan<T>::Do(owner->lastZone, N, offsetX, owner, (void(*)())Scan<T>::Do);
	}
	/*

	*/
	void MenuZoneDisable()
	{
		ItemData<T::sub_type> &d = Singleton<ItemData<T::sub_type>>::Instance();
		d.cancelOperatorSensor[N][owner->lastZone] ^= true;
		compute.Recalculation();
		RECT r;
		GetClientRect(hWnd, &r);
		{
			TSize s = {hWnd, WM_SIZE, 0, (WORD)r.right, (WORD)r.bottom};
			(*(Parent::Parent *)this)(s);
		}
		typedef TL::TypeAt<T::viewers_list, 0>::Result TopViewer;
		TopViewer &x = owner->viewers.get<TopViewer>();
		GetClientRect(x.hWnd, &r);
		{
			TSize s = {x.hWnd, WM_SIZE, 0, (WORD)r.right, (WORD)r.bottom};
			(*(TopViewer::Parent *)&x)(s);
		}
		RepaintWindow(owner->hWnd);
	}

	void operator()(TRButtonDown &l);	
};

template<class T, void(T::*)()>struct MenuNum{};

template<class T, void(T::*X)()>struct TopMenu<MenuNum<T, X> >
{
	typedef NullType list;	
};

template<class T>struct NameMenu<TopMenu<MenuNum<T, &T::MenuItemScan> > >{wchar_t *operator()(HWND){return L"Просмотр сигнала";}};
template<class T>struct NameMenu<TopMenu<MenuNum<T, &T::MenuZoneDisable> > >{wchar_t *operator()(HWND){return L"Изменить статус датчика в зоне";}};

template<class T, void(T::*X)()>struct Event<TopMenu<MenuNum<T, X> >> 	
{										
	static void Do(HWND h)				
	{									
		(((T *)GetWindowLongPtr(h, GWLP_USERDATA))->*X)();
	}									
};										

template<class T, int N>void Line<T, N>::operator()(TRButtonDown &l)
{
	typedef TL::MkTlst<	
		TopMenu<MenuNum<Line<T, N>, &Line<T, N>::MenuItemScan> >	
		, TopMenu<MenuNum<Line<T, N>, &Line<T, N>::MenuZoneDisable> >
	>::Result items_list;
	PopupMenu<items_list>::Do(l.hwnd, l.hwnd);
}

namespace
{
	template<class O, class P>struct __scan__
	{
		bool operator()(O *o, P *p)
		{
			return true;
		}
	};

	struct __scan_data__
	{
		int channel;
		int zone;
		int offs;
		int count;
		USPC7100_ASCANDATAHEADER *scan;
	};

	template<template<class, int>class L, class T, int N, class P>struct __scan__<L<T, N>, P>
	{
		typedef typename L<T, N> O;
		bool operator()(O *o, P *p)
		{
			if(N == p->channel)
			{
				o->dataViewer.Do(p->zone, p->channel);
				p->scan = o->dataViewer.scan[p->offs];
				p->count = o->dataViewer.count;
				typedef typename O::Parent::Parent TWhell;

				o->offsetX = p->offs;
				TMouseWell w = {o->hWnd, WM_MOUSEWHEEL};
				(*(TWhell *)o)(w);
				return false;
			}
			return true;
		}
	};

	template<class T>struct __for_label__
	{
		wchar_t *operator()(USPC7100_ASCANDATAHEADER *d, ScanWindow &s)
		{
			return s.label.buffer;
		}
	};

	template<>struct __for_label__<Thickness>
	{
		wchar_t buffer[128];
		wchar_t *operator()(USPC7100_ASCANDATAHEADER *d, ScanWindow &s)
		{
			if(0 != IsBadReadPtr(d, sizeof(d))) return L"";
			ItemData<Thickness> &t = Singleton<ItemData<Thickness> >::Instance();
			wchar_t s1[256];
			wchar_t s2[256];
			s1[0] = '\0';
			s2[0] = '\0';
			if(d->hdr.G1Tof)
			{
				double tmp = 2.5e-6 * d->hdr.G1Tof * t.param[d->Channel].get<gate1_TOF_WT_velocity>().value;
				wsprintf(s1, L"<ff>толщина1 <ffffff>%s", OneDigit(tmp)());
				if(d->hdr.G2Tof)
				{
					double tmp = 2.5e-6 * d->hdr.G2Tof * t.param[d->Channel].get<gate2_TOF_WT_velocity>().value;
					wsprintf(s2, L"<ff>толщина2 <ffffff>%s", OneDigit(tmp)());
				}
			}
			wsprintf(buffer, L"%s %s" , s1, s2);
			return buffer;
		}
	};
#define USPC(name) double _##name = uspc.param[d->Channel].get<name>().value;
	template<class T>struct __gates__
	{
		void operator()(ScanWindow &s, USPC7100_ASCANDATAHEADER *d)
		{
			try
			{
                if(0 != IsBadReadPtr(d, sizeof(d))) return;
				s.chart.items.get<ScanWindow::Gate1>().visible = true;
				s.chart.items.get<ScanWindow::GateIF>().visible = false;
				s.chart.items.get<ScanWindow::GateIFBorder>().visible = false;
				s.chart.items.get<ScanWindow::Gate1Border>().visible = false;
				s.chart.items.get<ScanWindow::Gate2>().visible = false;
				s.chart.items.get<ScanWindow::Gate2Border>().visible = false;
				// todo  расчёт гайтов для отрисовки
				ItemData<T> &uspc = Singleton<ItemData<T>>::Instance();
				USPC(scope_range);
				USPC(scope_offset);
				s.chart.minAxesX = _scope_offset;
				s.chart.maxAxesX = _scope_offset + _scope_range;

				ScanWindow::Gate1 &g1 = s.chart.items.get<ScanWindow::Gate1>();

				USPC(gate1_width);

				USPC(gate1_position);

				USPC(gate1_level);

				g1.x = _gate1_position;// + _scope_offset;
				g1.width = _gate1_width;
				double offs = 0.005 * d->hdr.G1Tof;
				g1.y = _gate1_level;


				int count = d->DataSize;
				if(0 == count) count = 500;
				double mash = s.chart.items.get<ScanWindow::Line>().mash = 0.001 * d->TimeEqu / count;
				s.chart.items.get<ScanWindow::Line>().count = int(_scope_range / mash);
				s.chart.items.get<ScanWindow::Line>().offset = 0;

				s.chart.items.get<ScanWindow::Gate1Border>().value = 0.005 * d->hdr.G1Tof - _scope_offset;
				int gate1Amp = d->hdr.G1Amp;

				int beg = int((_gate1_position - _scope_offset) / mash);
				int end = int((_gate1_position + _gate1_width - _scope_offset) / mash);
				if(beg < 0 || end < 0)
				{
					beg = int((_gate1_position) / mash);
					end = int((_gate1_position + _gate1_width) / mash);
				}		

				wchar_t buf[128];
				buf[0] = 0;
				int amp = 0;
				for(int i = beg; i < end; ++i)
				{
					if(d->Point[i] > amp)
					{
						amp	= d->Point[i];	
					}
				}

				wsprintf(buf, L"<ff>Амплитуда %d", amp);
				s.label = buf;
			}
			catch(...){}
		}
	};

	template<>struct __gates__<Thickness>
	{
		void operator()(ScanWindow &s, USPC7100_ASCANDATAHEADER *d)
		{
			if(0 != IsBadReadPtr(d, sizeof(d))) return;
			ItemData<Thickness> &uspc = Singleton<ItemData<Thickness>>::Instance();

			s.chart.items.get<ScanWindow::GateIFBorder>().visible = false;
			s.chart.items.get<ScanWindow::Gate1Border>().visible = false;
			s.chart.items.get<ScanWindow::GateIF>().visible = false;
			s.chart.items.get<ScanWindow::Gate1>().visible = false;
			s.chart.items.get<ScanWindow::Gate2Border>().visible = false;
			s.chart.items.get<ScanWindow::Gate2>().visible = false;

			ScanWindow::GateIF &gif = s.chart.items.get<ScanWindow::GateIF>();
			gif.visible = true;

			// todo  расчёт гайтов для отрисовки
			USPC(scope_range);
			USPC(scope_offset);
			s.chart.minAxesX = _scope_offset;
			s.chart.maxAxesX = _scope_offset + _scope_range;

			USPC(gateIF_position);
			USPC(gateIF_width);

			USPC(gateIF_level);

			gif.x = _gateIF_position;
			dprint("gif %f\n", gif.x);
			gif.width = _gateIF_width;
			gif.y = _gateIF_level;

			ScanWindow::Gate1 &g1 = s.chart.items.get<ScanWindow::Gate1>();

			USPC(gate1_width);

			USPC(gate1_position);

			USPC(gate1_level);

			ScanWindow::Gate2 &g2 = s.chart.items.get<ScanWindow::Gate2>();

			USPC(gate2_width);

			USPC(gate2_position);

			USPC(gate2_level);

			int count = d->DataSize;
			if(0 == count) count = 500;
			double mash = s.chart.items.get<ScanWindow::Line>().mash = 0.001 * d->TimeEqu / count;
			s.chart.items.get<ScanWindow::Line>().count = int(_scope_range / mash);
			s.chart.items.get<ScanWindow::Line>().offset = 0;

			double GIFTof = s.chart.items.get<ScanWindow::GateIFBorder>().value = 0.005 * d->hdr.GIFTof;
			dprint("GateIFBorder %f\n", GIFTof);
			s.chart.items.get<ScanWindow::Gate1Border>().value = 0.005 * (d->hdr.G1Tof + d->hdr.GIFTof);
			if(0 != d->hdr.GIFTof)
			{
				int i = 0;
				for(; i < count; ++i)
				{
					if(d->Point[i] > _gateIF_level)
					{
						s.chart.items.get<ScanWindow::GateIFBorder>().visible = true;
						s.chart.items.get<ScanWindow::GateIF>().visible = true;
						break;
					}
				}
		//		if(0 != d->hdr.G1Tof)
				{
					s.chart.items.get<ScanWindow::Gate1Border>().visible = true;
					s.chart.items.get<ScanWindow::Gate1>().visible = true;
					double x = 5e-3 * d->hdr.GIFTof;
					g1.x = _gate1_position + x;// - s.chart.items.get<ScanWindow::Line>().offset * mash;
					g1.width = _gate1_width;
					g1.y = _gate1_level;
					double g1B = s.chart.items.get<ScanWindow::Gate1Border>().value = 0.005 * (d->hdr.G1Tof + d->hdr.GIFTof);
					dprint("g1B %f\n", g1B);

		//			if(0 != d->hdr.G2Tof)
					{
						s.chart.items.get<ScanWindow::Gate2Border>().visible = true;
						s.chart.items.get<ScanWindow::Gate2>().visible = true;
						double x = 5e-3 * d->hdr.GIFTof;
						g2.x = _gate2_position + x;// - s.chart.items.get<ScanWindow::Line>().offset * mash;
						g2.width = _gate2_width;
						g2.y = _gate2_level;
						s.chart.items.get<ScanWindow::Gate2Border>().value = 0.005 * (d->hdr.G2Tof + d->hdr.GIFTof);
					}
				}
			}
		}
	};
#undef USPC

	template<class T> struct Scan
	{
		static bool Do(int zone, int sens, int offs, void *o, void(*ptr)())
		{
			if(NULL == FindWindow(WindowClass<T>()(), 0))return false;
			typedef typename T::sub_type Ascan;
			ItemData<Ascan> &data = Singleton<ItemData<Ascan>>::Instance();
			if(data.currentOffsetZones <= zone) return false;
			
			if(offs < 0)
			{
				offs = 0;
			}
			__scan_data__ d = {sens, zone, offs, NULL};
			TL::find<typename T::viewers_list, __scan__>()(&((T *)o)->viewers, &d);
			if(offs >= d.count) offs = d.count - 1;
			if(NULL != d.scan)
			{
				ScanWindow &s = Singleton<ScanWindow>::Instance();
				__gates__<Ascan>()(s,  d.scan);
				s.Open(
					zone
					, sens
					, offs
					, Title<Ascan>()()
					, __for_label__<Ascan>()(d.scan, s)
					, d.scan
					, o
					, ptr
					);
				return true;
			}
			return false;
		}
	};
}

