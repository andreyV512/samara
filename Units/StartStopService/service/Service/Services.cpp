#include "stdafx.h"
#include "Services.h"
#include <wbemidl.h>
#include <stdio.h>

#pragma warning(disable : 4996)
namespace Service
{

struct close_handle
{
	SC_HANDLE h;
	close_handle(SC_HANDLE h) : h(h){}
	~close_handle(){if(NULL != h)CloseServiceHandle(h);}
};

void Start()
{
	SC_HANDLE h = OpenSCManager( NULL, NULL,  SC_MANAGER_ALL_ACCESS);
	SERVICE_STATUS  ssp = {};

	SC_HANDLE hService = OpenService( 
        h,        
        L"USPC7100WinService",          
        SERVICE_START | 
        SERVICE_QUERY_STATUS | 
        SERVICE_ENUMERATE_DEPENDENTS);  
   StartService(
            hService,  // дескриптор службы 
            0,           // число параметров 
            NULL);
	CloseServiceHandle(hService);
	CloseServiceHandle(h);
}

void Stop()
{
	SC_HANDLE h = OpenSCManager( NULL, NULL,  SC_MANAGER_ALL_ACCESS);
	SERVICE_STATUS  ssp = {};

	SC_HANDLE hService = OpenService( 
        h,        
        L"USPC7100WinService",          
        SERVICE_STOP | 
        SERVICE_QUERY_STATUS | 
        SERVICE_ENUMERATE_DEPENDENTS);  

	ControlService( hService, 
                            SERVICE_CONTROL_STOP,
                            (LPSERVICE_STATUS) &ssp );
	CloseServiceHandle(hService);
	CloseServiceHandle(h);
}

void Status(char *s)
{
	SC_HANDLE h = OpenSCManager( NULL, NULL,  SC_MANAGER_ALL_ACCESS);
	close_handle close_handle0(h);
	if (h == NULL)
	{
		sprintf(s, "OpenSCManager failed %d\n", GetLastError());
		printf("%s\n", s);
		return;
	}
	SC_HANDLE hService = OpenService(h,  L"USPC7100WinService", SERVICE_QUERY_STATUS);
	close_handle close_handle1(hService);
	if (hService == NULL)
	{
		sprintf(s, "OpenService failed %d\n", GetLastError());
		printf("%s\n", s);
		return;
	}

	SERVICE_STATUS Status;
	// получаем состояние службы
	if (!QueryServiceStatus(hService, &Status))
	{
		sprintf(s, "QueryServiceStatus failed %d\n", GetLastError());
		printf("%s\n", s);
		//    CloseServiceHandle(hService);
		return;
	}
	char *mess = "Unknown";
	switch (Status.dwCurrentState)
	{
	case SERVICE_STOPPED:
		mess = "Stopped";
		break;
	case SERVICE_RUNNING:
		mess = "Running";
		break;
	case SERVICE_PAUSED:
		mess = "Paused";
		break;
	case SERVICE_STOP_PENDING:
		mess = "Stop Pending";
		break;
	case SERVICE_START_PENDING:
		mess = "Start Pending";
		break;
	case SERVICE_PAUSE_PENDING:
		mess = "Pause Pending";
		break;
	case SERVICE_CONTINUE_PENDING:
		mess = "Continue Pending";
		break;        
	}
	sprintf(s, "%s", mess);
	printf("%s\n", s);
}
}