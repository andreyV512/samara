#pragma once
#include "DiffApp/App.h"
#include "App/AppBase.h"
#include "USPC\uspc7100_exports.h"

///	������ ������ � ����� USPC
class USPCData
{
public:
	int currentOffsetFrames;   ///<  ����� ���������� ����� 
	int currentOffsetZones;	   ///< ����� �������� ����� � ����
	static USPC7100_ASCANDATAHEADER ascanBuffer[App::count_frames];	///<��������� �����
	int offsets[App::count_zones];  ///< �������� ������ �� �����
	int offsSensor[App::count_sensors];
	char commonStatus[App::count_zones];					///< ����� ������ �� �����
	bool cancelOperatorSensor[App::count_sensors][App::count_zones];
	double samplesPerZone;
	int deadZoneSamplesBeg, deadZoneSamplesEnd;
	void Start();///< ��������� ����� ������� ����� ����� ������ � �����
	UCHAR *CurrentFrame(); ///<�������� � ������� buffer ������ ����� ������
	void OffsetCounter(int offs);
	void SamplesPerZone(int tubeLength, int deadArea0, int deadArea1);
};

#define USPC_PARAM(name, val)struct name\
{\
	double value;\
	name(): value(val){}\
	char *Name(){return #name;}\
};

USPC_PARAM(scope_range    , 12.01)
USPC_PARAM(scope_offset   , 40.01)
USPC_PARAM(gateIF_position, 41.01)
USPC_PARAM(gateIF_width   , 2.01)
USPC_PARAM(gateIF_level   , 20.01)
USPC_PARAM(gate1_width    , 7.31)
USPC_PARAM(gate1_position , 1.21)
USPC_PARAM(gate1_level 	  , 30.01)
USPC_PARAM(gate1_TOF_WT_velocity , 6400.01)

USPC_PARAM(gate2_width    , 7.32)
USPC_PARAM(gate2_position , 1.22)
USPC_PARAM(gate2_level 	  , 30.02)
USPC_PARAM(gate2_TOF_WT_velocity , 6400.02)

#undef USPC_PARAM

class USPCViewerData: public USPCData
{
public:
	double buffer[App::count_sensors][App::count_zones];	///<����������� ������ �������� �� �������� � �����
	char status[App::count_sensors][App::count_zones];	///< ������ ������ �� �������� � �����	
	typedef TL::MkTlst<
		scope_range    
		, scope_offset   
		, gate1_width    
		, gate1_position 
		, gate1_level 	  
		, gate1_TOF_WT_velocity
	>::Result param_list;
	typedef TL::Factory<param_list> TParam;
	TParam param[App::count_sensors]; 
};

class USPCViewerThicknessData: public USPCData
{
public:
	double bufferMin[App::count_zones];	///<����������� ������ �������� �� �������� � �����
	double bufferMax[App::count_zones];	///<����������� ������ �������� �� �������� � �����

    double movingAverageMin[App::count_zones];	///<����������� �������  �������� �� �������� � �����
	double movingAverageMax[App::count_zones];	///<����������� �������  �������� �� �������� � �����

	typedef TL::MkTlst<
		scope_range    
		, scope_offset   
		, gateIF_position
		, gateIF_width   
		, gateIF_level   
		, gate1_width    
		, gate1_position 
		, gate1_level 	  
		, gate1_TOF_WT_velocity

		, gate2_width    
		, gate2_position 
		, gate2_level 	  
		, gate2_TOF_WT_velocity
	>::Result param_list;
	typedef TL::Factory<param_list> TParam;
	TParam param[App::count_sensors]; 
};

template<class T>struct ItemData;
template<>struct ItemData<Cross>: USPCViewerData{}; 
template<>struct ItemData<Long>: USPCViewerData{};
template<>struct ItemData<Thickness>: USPCViewerThicknessData{};
