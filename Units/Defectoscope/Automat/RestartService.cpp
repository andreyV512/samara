#include "stdafx.h"
#include "RestartService.h"
#include "ProtocolTCP\GetHostByName.h"
#include "Communication/ServiceProtocol\ServiceProtocol.h"

namespace
{
	const wchar_t *hostName = L"10-012";
}

bool RestartService()
{
	GetHostByName getHostByName;
	char status[128];
	status[0] = '\0';

	if(!StopProtocol::Client::Do(getHostByName((wchar_t *)hostName), 8989))return false;
	Sleep(1500);
	for(int i = 0; i < 10; ++i)
	{
		if(StatusProtocol::Client::Do(getHostByName((wchar_t *)hostName), 8989, status))
		{
			return false;
		}
		Sleep(1500);
		if(0 == strcmp(status, "Stopped")) break;
	}

	if(0 != strcmp(status, "Stopped")) return false;

	RunProtocol::Client::Do(getHostByName((wchar_t *)hostName), 8989);
	Sleep(1500);
	for(int i = 0; i < 10; ++i)
	{
		if(StatusProtocol::Client::Do(getHostByName((wchar_t *)hostName), 8989, status))
		{
			return false;
		}
		Sleep(1500);
		if(0 == strcmp(status, "Running")) break;
	}
	return 0 == strcmp(status, "Running");
}