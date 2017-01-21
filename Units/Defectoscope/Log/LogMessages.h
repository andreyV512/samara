#pragma once

#define __double_1 " %.1f" 
#define __double_2 " %.2f"
#define __int " %d"
#define __void ""
#define __bool " %s"
#define __const_char_ptr " %s"
#define _cat(a, b) a##b

#define MESS(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe);}\
};
#define MESSX(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return txt;}\
};
#define MESS1(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_1);}\
};
#define MESS2(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_2);}\
};

namespace LogMess
{
	enum ID
	{
		StartSycle
		, StopSycle
		, OverheadTemperature
		, ProgramOpen

		, TimeoutPipe
		, AlarmSensorReadError
		, AlarmUSPC
		, AlarmRestartServiceError
		, AutomatMode
		, SingleMode

		, ProgramClosed
		, ErrStop
		, AlarmExitTubeInUnit
		, AlarmExitDirectionChainOn
		, AlarmExitInverterFaulty

		, AlarmExitRunBitIn			 
		, AlarmExitControlCircuitBitIn 
		, AlarmExitWorkBitIn			 
		, AlarmExitSycleBitIn		  

		, InfoOnWorkBitIn 
		, InfoOffWorkBitIn
		, InfoDataReceive
		, InfoOnResultBitOut
		, InfoReadyBitOut	            
		, InfoOnSycleBitIn  
		, InfoRotationalSpeedSensorsStart
		, InfoOnRunBitIn                 
		, InfoControlBitOut              
		, InfoOnSQ1BitIn	                
		, InfoPowerScannerBitOut         
		, InfoOnSQ3BitIn	                
		, InfoOffSQ1BitIn                
		, InfoOffPowerScannerBitOut         
		, InfoOffSQ3BitIn 
		, InfoUserStop
		, InfoDataCollectionComplete
		, InfoDataCollection
		, InfoBase
		//, Info

		, SQ1BitIn           
		, SQ3BitIn           
		, StateBBitIn        
		, StateABitIn        
		, RunBitIn           
		, ControlCircuitBitIn
		, WorkBitIn          
		, SycleBitIn         
		, BaseBitIn 
		, PowerInverterBitOut
		, SpeedRLBitOut      
		, SpeedRMBitOut      
		, SpeedRHBitOut      
		, STFBitOut          
		, PowerScannerBitOut
		, ReadyBitOut        
		, ControlBitOut      
		, ResultBitOut   
		, WaitControlCircuitBitIn
		, PowerBMOn
		, WaitCycle
		, WaitReady
		, WaitControl

		, AlarmControlCircuts
		, AlarmCycle
		, AlarmBase

		//, ContineCycleOk
		//, ContineCycleBrak

		, CycleOk  
		, CycleBrak

		, AlarmNoDataCollection
		, InfoDataSaved
		, AlarmBufferOverflow

		, max_row
	};

	//typedef const char * const_char_ptr;

	static const int green = 0xff00;
	static const int blue  = 0xff0000;
	static const int black = 0x0;
	static const int white = 0xffffff;
	static const int red = 0xff;
	static const int yellow = 0xffff;

	template<int >struct Row;
	///\brief сопоставление числу сообщениея
	///\param число из перечисления ID(см. выше)
	///\param тип дополнительного параметра
	///\param сообщение
	///\param цвет фона
	///\param цвет шрифта
	MESS(StartSycle			          , void  , "Цикл старт"                     , green, black)
    MESS(StopSycle			          , void  , "Цикл стоп"				        , blue , white)
	MESS1(OverheadTemperature         , double, "Температура превышена"	        , red  , yellow)
	MESS(ProgramOpen		          , void  , "Программа открыта"              , black, white)
							          
	MESS(TimeoutPipe		          , void  , "Превышенно время ожидания", red  , yellow)
	//MESS(AlarmUSPC                    , void  , "Авария!!! Плата ультрозвукового контроля не исправна", red, yellow)
	MESSX(AlarmUSPC                    , void  , "Авария!!! Платы ультразвукового контроля %X", red, yellow)
	MESSX(AlarmSensorReadError         , void  , "Авария!!! Чтения данных с платы ультразвукового контроля %X", red, yellow)

	MESS(AlarmRestartServiceError     , void  , "Авария!!! Сервис \"USPS7100WinService\" не перезапускается", red, yellow)
							          
	MESS(AutomatMode		          , void  , "Режим \"Автоматический\""		, blue , white)
	MESS(SingleMode			          , void  , "Режим \"Одиночное измерение\""	, blue , white)
							          
	MESS(ProgramClosed		          , void  , "Программа закрыта"	, red  , yellow)
							          
	MESS(ErrStop                      , void  , "Прерывание цикла измерения", red, yellow)

	MESS(AlarmExitTubeInUnit          , void, "Труба в модуле, выполните выгон", red  , yellow)
	MESS(AlarmExitDirectionChainOn    , void, "Включите цепи управления", red  , yellow)
	MESS(AlarmExitInverterFaulty      , void, "Частотный преодразователь не включён", red  , yellow)

	MESS(AlarmExitRunBitIn			  , void, "Ошибка частотного преобразователя\"", red  , yellow)
	MESS(AlarmExitControlCircuitBitIn , void, "Нет сигнала \"Цепи управления\""	   , red  , yellow)
	MESS(AlarmExitWorkBitIn			  , void, "Нет сигнала \"Работа\""			   , red  , yellow)
	MESS(AlarmExitSycleBitIn		  , void, "Нет сигнала \"Цикл\""			   , red  , yellow)

	MESS(AlarmNoDataCollection        , void, "Внимание!!! Нет сбора данных", red, yellow)
	MESS(AlarmBufferOverflow, void, "Внимание!!! Буфер переполнен", red, yellow)

	MESS(WaitControlCircuitBitIn      , void, "Ожидание сигнала \"Цепи управления\"", blue, white)

	MESS(PowerBMOn, void, "Включено \"ПИТАНИЕ БМ\"", blue, white)
	MESS(WaitCycle, void, "Ожидание входного сигнала \"Цикл\" и \"Готовность\"", blue, white)
	MESS(WaitReady, void, "Ожидание входного сигнала \"Готовность\"", blue, white)
	MESS(WaitControl, void, "Ожидание входного сигнала \"Контроль\"", blue, white) 

	MESS(AlarmControlCircuts, void, "Авария!!! Снят сигнал \"Цепи управления\"", red, yellow)
	MESS(AlarmBase,           void, "Авария!!! Нет сигнала \"БАЗА\"", red, yellow)
	MESS(AlarmCycle			, void, "Авария!!! Снят сигнал \"Цикл\"", red, yellow)

	//MESSX(ContineCycleOk, double, "Результат контроля \"ГОДНО\". Длина трубы %.1f м. Для продолжения нажмите кнопку \"Повтор\"", blue, white)
	//MESSX(ContineCycleBrak, double, "Результат контроля \"БРАК\". Длина трубы %.1f м. Для продолжения нажмите кнопку \"Повтор\"", red, yellow)
		
	MESSX(CycleOk  , double, "Результат контроля \"ГОДНО\". Длина трубы  %.2f м.", blue, white)
	MESSX(CycleBrak, double, "Результат контроля \"БРАК\". Длина трубы  %.2f м.", red, yellow)


	MESS(InfoOnWorkBitIn                , void, "Ожидание сигнала \"Работа\""					  , blue , white)
	MESS(InfoOffWorkBitIn				, void, "Ожидание снятия сигнала \"Работа\""			  , green, blue)
	MESS(InfoDataReceive				, void, "Данные с дефектоскопа приняты"                   , blue, yellow)
	MESS(InfoReadyBitOut	            , void, "Выставлен сигнал \"Готовность\""				  , blue , white)
	MESS(InfoOnResultBitOut             , void, "Выставлен сигнал \"Результат\""                 , blue , white)
	MESS(InfoOnSycleBitIn               , void, "Ожидание сигнала \"Цикл\""					  , blue , white)
	MESS(InfoRotationalSpeedSensorsStart, void, "Включение вращения модуля датчиков сканирования", blue , white)
	MESS(InfoOnRunBitIn                 , void, "Ожидание раскрутки модуля датчиков сканирования", blue , white)
	MESS(InfoControlBitOut              , void, "Выставлен сигнал \"Контроль\""				  , blue , white)
	MESS(InfoOnSQ1BitIn	                , void, "Ожидание трубы на датчике SQ1"				  , blue , white)
	MESS(InfoPowerScannerBitOut         , void, "Включение сканирующего устройства"			  , blue , white)
	MESS(InfoOnSQ3BitIn	                , void, "Ожидание трубы на датчике SQ3"				  , blue , white)
	MESS(InfoOffSQ1BitIn                , void, "Ожидание съезда трубы с датчика SQ1"			  , blue , white)
	MESS(InfoOffPowerScannerBitOut      , void, "Отключение сканирующего устройства"		      , blue , white)
	MESS(InfoOffSQ3BitIn                , void, "Ожидание съезда трубы с датчика SQ3"			  , blue , white)
	MESS(InfoUserStop					, void, "Оператор вышел из цикла сбора данных"			  , red  , yellow)

	MESS(InfoDataCollectionComplete		, void, "Сбор данных закончен"                           , green, white);
	MESS(InfoDataCollection     		, void, "Сбор данных"                           , blue, white);
	MESS(InfoBase, void, "Сработал датчик \"База\"", blue, white);

	MESS(InfoDataSaved, void, "Данные сохранены", green, white);

	MESS(SQ1BitIn             , bool, "SQ1BitIn            ", white, black)
	MESS(SQ3BitIn             , bool, "SQ3BitIn            ", white, black)
	MESS(StateBBitIn          , bool, "StateBBitIn         ", white, black)
	MESS(StateABitIn          , bool, "StateABitIn         ", white, black)
	MESS(RunBitIn             , bool, "RunBitIn            ", white, black)
	MESS(ControlCircuitBitIn  , bool, "ControlCircuitBitIn ", white, black)
	MESS(WorkBitIn            , bool, "WorkBitIn           ", white, black)
	MESS(SycleBitIn           , bool, "SycleBitIn          ", white, black)
	MESS(BaseBitIn 			  , bool, "BaseBitIn 		   ", white, black)
	MESS(PowerInverterBitOut  , bool, "PowerInverterBitOut ", white, black)
	MESS(SpeedRLBitOut        , bool, "SpeedRLBitOut       ", white, black)
	MESS(SpeedRMBitOut        , bool, "SpeedRMBitOut       ", white, black)
	MESS(SpeedRHBitOut        , bool, "SpeedRHBitOut       ", white, black)
	MESS(STFBitOut            , bool, "STFBitOut           ", white, black)
	MESS(PowerScannerBitOut	  , bool, "PowerScannerBitOut  ", white, black)
	MESS(ReadyBitOut          , bool, "ReadyBitOut         ", white, black)
	MESS(ControlBitOut        , bool, "ControlBitOut       ", white, black)
	MESS(ResultBitOut   	  , bool, "ResultBitOut   	 "  , white, black)

	class FactoryMessages
	{
		struct Inner;
		Inner &inner;
		FactoryMessages();
	public:
		void StartTime();
		bool Color(int i, unsigned &backColor, unsigned &textColor);
		bool Text(int i, char *buf, double val);
		static FactoryMessages &Instance();
	};
}
#undef MESS
#undef MESS1
#undef MESS2
#undef __double_1 
#undef __double_2 
#undef __int 
#undef __void 
#undef _cat


