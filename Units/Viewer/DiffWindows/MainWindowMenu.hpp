#pragma once
#include "App/config.h"
#include "Ini/WindowsPosition.h"
#include "Dialogs/Dialogs.h"
#include "Windows/CrossThresholdWindow.h"
#include "Windows/LongThresholdWindow.h"
#include "Windows/ThicknessThresholdWindow.h"
#include "Windows/Common.h"

namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile: LoadDataDlg{};//{static void Do(HWND h){zprint("");}};
	struct SaveDateFile: StoredDataDlg{};//{static void Do(HWND h){zprint("");}};
	struct Compute     : RecalculationDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainExit
	{
		static void Do(HWND h)
		{
			TClose c = {h, WM_CLOSE, 0, 0};
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"Загрузить данные", LoadDateFile)
	MENU_ITEM(L"Сохранить данные", SaveDateFile)
	MENU_ITEM(L"Перерасчёт", Compute)
	MENU_ITEM(L"Выход", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<LoadDateFile>
			//, MenuItem<SaveDateFile>
			, Separator<0>
			, MenuItem<Compute>
			, Separator<1>
			, MenuItem<MainExit>
		>::Result list;
	};
	//------------------------------------------------------------------------
	struct MainOptionTypeSize{};
	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct Tresholds {};
	struct Thickness{};
	struct DeadZones         : DeadZonesDlg{};//{static void Do(HWND h){zprint("");}};
	//struct AllowableThickness{static void Do(HWND h){zprint("");}};
	//struct RotationalSpeed   {static void Do(HWND h){zprint("");}};
							 						

	struct CrossThresholdWindow__: Common::OpenWindow<CrossThresholdWindow>{};
	struct LongThresholdWindow__: Common::OpenWindow<LongThresholdWindow>{};
	struct ThicknessThresholdWindow__: Common::OpenWindow<ThicknessThresholdWindow>{};

	struct BrackStrobe2__: BrackStrobe2Dlg{};//{static void Do(HWND h){zprint("");}};

	struct MedianFiltre           : MedianFiltreDlg{};//{static void Do(HWND h){zprint("");}};

	template<>struct SubMenu<Tresholds>
	{
		typedef TL::MkTlst<
			MenuItem<CrossThresholdWindow__>
			, MenuItem<LongThresholdWindow__>
			, SubMenu<Thickness>
		>::Result list;
	};

   // MENU_ITEM(L"Пороги отбраковки", ThicknessTreshold)
	MENU_TEXT(L"Пороги отбраковки", SubMenu<Tresholds>)
	MENU_ITEM(L"Поперечные пороги", CrossThresholdWindow__)
	MENU_ITEM(L"Продольные пороги", LongThresholdWindow__)
	MENU_ITEM(L"Пороги толщины", ThicknessThresholdWindow__)
	MENU_ITEM(L"Мёртвые зоны", DeadZones)

	MENU_ITEM(L"Медианный фильтр", MedianFiltre)
	MENU_ITEM(L"Расслоение", BrackStrobe2__)
	MENU_TEXT(L"Толщина", SubMenu<Thickness>)

	

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
		     SubMenu<Tresholds>
			, MenuItem<DeadZones>
			//, MenuItem<AllowableThickness>
			, MenuItem<MedianFiltre>
			//, MenuItem<RotationalSpeed>
			//, Separator<0>
			//, MenuItem<MainCreateTypesize>
			//, MenuItem<MainDeleteTypeSize>
		>::Result list;
	 };
	//--------------------------------------------------------------------------------
	struct Options{};
	MENU_TEXT(L"Настройки", TopMenu<Options>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct WindowPosition    : WindowPositionDlg<MainWindow>{};//{static void Do(HWND h){zprint("");}};

	MENU_ITEM(L"Сохранить координаты окна", WindowPosition)

	template<>struct TopMenu<Options>
	{
		typedef TL::MkTlst<
			MenuItem<WindowPosition>
		>::Result list;		
	};
	//-------------------------------------------------------------------------------------------------------
	struct Setting{};
	MENU_TEXT(L"Установка", TopMenu<Setting>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct AnalogPlate            : AnalogPlateDlg{};//{static void Do(HWND h){zprint("");}};
	struct DiscretePlate	      {};//static void Do(HWND h){zprint("");}};
	struct DiscretePlateInputs    : InputsDlg{};//{static void Do(HWND h){zprint("");}};
	struct DiscretePlateOutputs   : OutputsDlg{};//{static void Do(HWND h){zprint("");}};
	struct ColorItems             : ColorItemsDlg{};//{static void Do(HWND h){zprint("");}};
	struct Coefficient            {static void Do(HWND h){zprint("");}};
	//struct MedianFiltre           : MedianFiltreDlg{};//{static void Do(HWND h){zprint("");}};
	struct Signal                 {static void Do(HWND h){zprint("");}};
	struct ACFBorder              {static void Do(HWND h){zprint("");}};
	//struct TestTab                : TestTabsDlg{};//{static void Do(HWND h){zprint("");}};
	struct OffsetsSensors: OffsetsSensorsDlg{};
	struct BaseLength : BaseLengthDlg{};
	
	MENU_TEXT(L"Дискретная плата", SubMenu<DiscretePlate>)
	MENU_ITEM(L"Входные порты", DiscretePlateInputs)
	MENU_ITEM(L"Выодные порты", DiscretePlateOutputs)
	MENU_ITEM(L"Коэффициенты пересчёта", Coefficient)
	
	MENU_ITEM(L"Аналоговая плата", AnalogPlate)
	MENU_ITEM(L"Параметры сигнала", Signal)
	MENU_ITEM(L"Границы АЧХ", ACFBorder)
	MENU_ITEM(L"Цвета", ColorItems)
	//MENU_ITEM(L"Тестирование окна с закладками", TestTab)
	MENU_ITEM(L"Смещение датчиков", OffsetsSensors)
	MENU_ITEM(L"Базовое расстояние", BaseLength)
	

	template<>struct TopMenu<Setting>
	{
		typedef TL::MkTlst<
			MenuItem<OffsetsSensors>
			, MenuItem<BaseLength>
			, MenuItem<ColorItems>
		>::Result list;		
	};
	// ----------------------------------------------------------------------------------------------------
	struct MainAbout : AboutWindowDlg{};//{static void Do(HWND h){zprint("");}};
	MENU_TEXT(L"О программе", TopMenu<MainAbout>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MENU_ITEM(L"О программе", MainAbout)

	template<>struct TopMenu<MainAbout>
	{
		typedef TL::MkTlst<
			MenuItem<MainAbout>
		>::Result list;
	};
	// --------------------------------------------------------------------------------------------------------- 
	/// \brief	Добавить меню пример использования
	//struct TestMenu{};
	//MENU_TEXT(L"TestMenu", TopMenu<TestMenu>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//struct TestSubMenu0{};
	//struct TestSubMenu1{static void Do(HWND){zprint("");}};
	//struct TestSubMenu2{static void Do(HWND){zprint("");}};
	//struct TestSubMenu3{static void Do(HWND){zprint("");}};
	//struct TestSubMenu4{static void Do(HWND){zprint("");}};
	//
	//MENU_ITEM(L"TestSubMenu0", TestSubMenu0)
	//MENU_ITEM(L"TestSubMenu1", TestSubMenu1)
	//MENU_ITEM(L"TestSubMenu2", TestSubMenu2)
	//MENU_ITEM(L"TestSubMenu3", TestSubMenu3)
	//MENU_ITEM(L"TestSubMenu4", TestSubMenu4)
	//
	//template<>struct TopMenu<TestMenu>
	//{
	//	typedef TL::MkTlst<
	//		MenuItem<TestSubMenu0>
	//		, MenuItem<TestSubMenu1>
	//		, MenuItem<TestSubMenu2>
	//		, MenuItem<TestSubMenu3>
	//		, MenuItem<TestSubMenu4>
	//	>::Result list;
	//};
	//---------------------------------------------------------------------------------------------------------------
	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionTypeSize>
		, TopMenu<Options>
		, TopMenu<Setting>
		, TopMenu<MainAbout>
	>::Result MainMenu;	
	}
