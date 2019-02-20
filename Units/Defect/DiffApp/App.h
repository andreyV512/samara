#pragma once
#include "USPC/uspc7100_exports.h"
#include "templates/typelist.hpp"
class MainWindow;

#define DEFECT_APP
struct Long{};
struct Cross{};
struct Thickness{};
struct ViewInterrupt{};
namespace USPC
{
	typedef TL::MkTlst<Thickness, Long, Cross>::Result items_list;
}

class App
{
public:
	static const int count_zones = 300;
	static const int count_sensors = 8;
	static const int zone_length = 50;

	static const int speed_bottom_label = 0;
	static const int number_party       = 1;
	static const int number_tube       =  2;
	static const int operator_name      = 3;

	static const int count_frames = 1200000;
	static const int count_frames_per_channel = count_frames / count_sensors;
	static const int thickness_number = 0;
	static const int long_number = 1;
	static const int cross_number = 2;
	static HANDLE ProgrammExitEvent;
	static HANDLE ProgrammContinueEvent;
	static HANDLE ProgrammStopEvent;
	static HANDLE ProgrammRunEvent;
	static bool measurementOfRunning;									
public:
	MainWindow &mainWindow;
	static const int &lengthCaretka;
	wchar_t nameFile[64];
public:
	App();
	void Init();
	void Destroy();
	void MainWindowTopLabel(wchar_t *);
	void MainWindowBottomLabel(int, wchar_t *);  
	void MainWindowUpdate();
	static HWND MainWindowHWND();
	static void InitCaretka();
	static const wchar_t *TitleApp(){return L"À¡“ \"¡Û‡Ì-5000\"";}
	static const wchar_t *Salt(){return L"Defect";};
};

extern App app;

template<class T>struct Singleton
{
	static T& Instance(){static T x; return x;}
};




