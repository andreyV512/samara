#include "stdafx.h"
#include "App.h"
#include "AppBase.h"
#include "1730/Device1730.h"
#include "Automat/RestartService.h"
#include "DebugMess.h"

bool App::Init()
{
   AppBase().Init();
   if(!device1730.Init(Singleton<Descriptor1730Table>::Instance().items.get<Descriptor1730>().value))
   {
	   char b[128];
	   CharToOemA("Не могу инициировать плату 1730\nДля закрытия нажмите \"ENTER\"", b);
	   dprint(b);
	   return false;
   }
   device1730.Write(Singleton<OutputBitTable>::Instance().items.get<oPowerBM>().value);
   Sleep(2000);
   if(!RestartService())
   {
	   char b[128];
	   CharToOemA("Нет связи с дефектоскопом\nДля закрытия нажмите \"ENTER\"", b);
	   dprint(b);
	   return false;
   }
   return true;
}

void App::Destroy()
{
	if(device1730.IsOpen())
	{
		device1730.Write(0);
		Sleep(500);
	}
}

Device1730 device1730;
