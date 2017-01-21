// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include "ServiceProtocol.h"
#include "GetHostByName.h"
#include "InitTcp.h"

void Menu()
{
	printf("1-Status\n2-Stop\n3-Start\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	wchar_t *hostName = L"ALCOA-BURAN5000";
	InitTcp  initTcp;
	GetHostByName getHostByName;
	Menu();
	while(true)
	{
		if(_kbhit())
		{
			switch(_getch())
			{
			case '1':
				StatusProtocol::Client::Do(getHostByName(hostName), 8989);
				break;
			case '2':
				StopProtocol::Client::Do(getHostByName(hostName), 8989);
				break;
			case '3':
				RunProtocol::Client::Do(getHostByName(hostName), 8989);
				break;
			default: Menu();
			}
		}
		else
		{
			Sleep(1000);
		}
	}
	//getchar();
	return 0;
}

