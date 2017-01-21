#include "stdafx.h"
#include "Services.h"

#pragma comment(lib, "mpr.lib")

namespace
{
	static const int RMLEN = 1024;
}

SC_HANDLE SCMConnect(
    IN PCTSTR pszMachine,
    IN PCTSTR pszUserName,
    IN PCTSTR pszPassword,
    IN DWORD dwAccess
    )
{
    if (pszMachine != NULL && pszUserName != NULL)
    {
      //  _ASSERTE(pszPassword != NULL);

        NETRESOURCE NetRes;
        DWORD dwStatus;
        TCHAR szRemoteName[RMLEN + 1];

        _sntprintf(szRemoteName, RMLEN + 1, _T("%s\\IPC$"),
                   pszMachine);

        NetRes.dwType = RESOURCETYPE_ANY;
        NetRes.lpLocalName = NULL;
        NetRes.lpRemoteName = szRemoteName;
        NetRes.lpProvider = NULL;

        // попробуем установить соединение с ресурсом IPC$ на
        // удаленной машине
        dwStatus = WNetAddConnection2(&NetRes, pszPassword, 
                                      pszUserName, 0);
        if (dwStatus != ERROR_SUCCESS)
            return SetLastError(dwStatus), FALSE;
    }

    // устанавливаем соединение с SCM
    return OpenSCManager(pszMachine, NULL, dwAccess);
}

//-----------------------------------------------------------------------
// DemoServiceStatus
//
//  Отображает текущее состояние службы.
//
//  Параметры:
//      hSCM           - handle SCM
//      pszServiceName - имя службы
//
//  Возвращает:
//      ноль - при успешном завершении, отличное от нуля значение - в
//      противном случае.
//

unsigned PrintError(wchar_t *txt, unsigned err)
{
	wprintf(L"%s  %d\n", txt, err);
	return err;
}

int DemoServiceStatus(
    IN SC_HANDLE hSCM, 
    IN PCTSTR pszServiceName
    )
{
   // _ASSERTE(hSCM != NULL);
   // _ASSERTE(pszServiceName != NULL);
    
    SC_HANDLE hService;
    SERVICE_STATUS Status;

    // открываем handle службы с доступом SERVICE_QUERY_STATUS
    hService = OpenService(hSCM, pszServiceName, SERVICE_QUERY_STATUS);
    if (hService == NULL)
        return PrintError(_T("OpenService failed"), GetLastError());

    // получаем состояние службы
    if (!QueryServiceStatus(hService, &Status))
    {
        PrintError(_T("QueryServiceStatus failed"), GetLastError());
        CloseServiceHandle(hService);
        return 1;
    }

    PCTSTR pszServiceType;
    PCTSTR pszInteractive;
    PCTSTR pszStatus;

    switch (Status.dwServiceType & ~SERVICE_INTERACTIVE_PROCESS)
    {
        case SERVICE_KERNEL_DRIVER:
            pszServiceType = _T("Kernel Driver");
            break;
        case SERVICE_FILE_SYSTEM_DRIVER:
            pszServiceType = _T("File System Driver");
            break;
        case SERVICE_WIN32_OWN_PROCESS:
            pszServiceType = _T("Win32 Own Process");
            break;
        case SERVICE_WIN32_SHARE_PROCESS:
            pszServiceType = _T("Win32 Share Process");
            break;
        default:
            pszServiceType = _T("Unknown");
            break;
    }

    if (Status.dwServiceType & SERVICE_INTERACTIVE_PROCESS)
        pszInteractive = _T("(Interactive)");
    else
        pszInteractive = _T("");

    switch (Status.dwCurrentState)
    {
        case SERVICE_STOPPED:
            pszStatus = _T("Stopped");
            break;
        case SERVICE_RUNNING:
            pszStatus = _T("Running");
            break;
        case SERVICE_PAUSED:
            pszStatus = _T("Paused");
            break;
        case SERVICE_STOP_PENDING:
            pszStatus = _T("Stop Pending");
            break;
        case SERVICE_START_PENDING:
            pszStatus = _T("Start Pending");
            break;
        case SERVICE_PAUSE_PENDING:
            pszStatus = _T("Pause Pending");
            break;
        case SERVICE_CONTINUE_PENDING:
            pszStatus = _T("Continue Pending");
            break;
        default:
            pszStatus = _T("Unknown");
            break;
    }

    // выводим состояние службы
    _tprintf(_T("Type:              %s %s\n"), pszServiceType, 
             pszInteractive);
    _tprintf(_T("State:             %s\n"), pszStatus);
    _tprintf(_T("Controls Accepted:\n"));

    if (Status.dwControlsAccepted & SERVICE_ACCEPT_STOP)
        _tprintf(_T("\tStop\n"));
    if (Status.dwControlsAccepted & SERVICE_ACCEPT_PAUSE_CONTINUE)
        _tprintf(_T("\tPause and Continue\n"));
    if (Status.dwControlsAccepted & SERVICE_ACCEPT_SHUTDOWN)
        _tprintf(_T("\tShutdown\n"));
    if (Status.dwControlsAccepted & SERVICE_ACCEPT_PARAMCHANGE)
        _tprintf(_T("\tParam Change\n"));
    if (Status.dwControlsAccepted & SERVICE_ACCEPT_NETBINDCHANGE)
        _tprintf(_T("\tNet Bind Change\n"));
    if (Status.dwControlsAccepted & SERVICE_ACCEPT_HARDWAREPROFILECHANGE)
        _tprintf(_T("\tHardware Profile Change\n"));
    if (Status.dwControlsAccepted & SERVICE_ACCEPT_POWEREVENT)
        _tprintf(_T("\tPower Event\n"));

    _tprintf(_T("Win 32 Exit Code:  %d\n"), Status.dwWin32ExitCode);
    _tprintf(_T("Service Exit Code: %d\n"), 
             Status.dwServiceSpecificExitCode);
    _tprintf(_T("Check Point:       %d\n"), Status.dwCheckPoint);
    _tprintf(_T("Wait Hint:         %d\n"), Status.dwWaitHint);


#if _WIN32_WINNT >= 0x500
    // получаем и отображаем расширенную информацию о состоянии

    SERVICE_STATUS_PROCESS StatusEx;
    DWORD cbNeeded;

    if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
                              (PBYTE)&StatusEx, sizeof(StatusEx),
                              &cbNeeded))
    {
        PrintError(_T("QueryServiceStatus failed"), GetLastError());
        CloseServiceHandle(hService);
        return 1;
    }

    PCTSTR pszSystemProcess;
    if (StatusEx.dwServiceFlags & SERVICE_RUNS_IN_SYSTEM_PROCESS)
        pszSystemProcess = _T("Yes");
    else
        pszSystemProcess = _T("No");

    _tprintf(_T("Process Id:        %d\n"), StatusEx.dwProcessId);
    _tprintf(_T("System Process:    %s\n"), pszSystemProcess);

#endif

    CloseServiceHandle(hService);
    return 0;
}

//---------------------------------------------------------------------------
// DemoServiceStopWithDependents
//
//  Stops the specified service and all its dependents.
//
//  Parameters:
//	  hSCM			 - SCM handle
//	  pszServiceName - service name
//
//  Returns:
//	  zero, if successful, nonzero - otherwise.
//
int
DemoServiceStopWithDependents(
	IN SC_HANDLE hSCM, 
	IN PCTSTR pszServiceName
	)
{
	//_ASSERTE(hSCM != NULL);
	//_ASSERTE(pszServiceName != NULL);

	SC_HANDLE hService;
	LPENUM_SERVICE_STATUS pStatus = NULL;
	DWORD cbNeeded;
	DWORD cServices = 0, i;

	// open service handle with SERVICE_ENUMERATE_DEPENDENTS access
	hService = OpenService(hSCM, pszServiceName, 
						   SERVICE_ENUMERATE_DEPENDENTS);
	if (hService == NULL)
		return PrintError(_T("OpenService failed"), GetLastError());

	// determine how much space we should allocate for the output
	// buffer
	if (!EnumDependentServices(hService, SERVICE_ACTIVE, NULL, 0,
							   &cbNeeded, &cServices))
	{
		if (GetLastError() != ERROR_MORE_DATA)
		{
			PrintError(_T("EnumDependentServices failed"), GetLastError());
			//_VERIFY(
				CloseServiceHandle(hService);
			//);
			return 1;
		}
	}

	if (cbNeeded > 0)
	{
		// allocate memory for the output buffer
		pStatus = (LPENUM_SERVICE_STATUS)LocalAlloc(LMEM_FIXED, cbNeeded);
		if (pStatus == NULL)
		{
			_tprintf(_T("Not enough memory\n"));
			//_VERIFY(
				CloseServiceHandle(hService);//);
			return 1;
		}

		// obtain the list of dependent services
		if (!EnumDependentServices(hService, SERVICE_ACTIVE, pStatus,
								   cbNeeded, &cbNeeded, &cServices))
		{
			PrintError(_T("EnumDependentServices failed"), GetLastError());

			LocalFree((HLOCAL)pStatus);
			//_VERIFY(
				CloseServiceHandle(hService);//);
			return 1;
		}
	}

	// stop each dependent service
	for (i = 0; i < cServices; i++)
	{
		_tprintf(_T("Stopping %s...\n"), pStatus[i].lpDisplayName);

		if (DemoServiceStop(hSCM, pStatus[i].lpServiceName))
			break;
	}

	LocalFree((HLOCAL)pStatus);
	//_VERIFY(
		CloseServiceHandle(hService);//);

	if (i < cServices)
		return 1;
	
	// stop the service itself
	_tprintf(_T("Stopping %s...\n"), pszServiceName);
	return DemoServiceStop(hSCM, pszServiceName);
}

//---------------------------------------------------------------------------
// DemoServiceStop
//
//  Stops the specified service.
//
//  Parameters:
//	  hSCM			 - SCM handle
//	  pszServiceName - service name
//
//  Returns:
//	  zero, if successful, nonzero - otherwise.
//
int DemoServiceStop(
	IN SC_HANDLE hSCM, 
	IN PCTSTR pszServiceName
	)
{
	//_ASSERTE(hSCM != NULL);
	//_ASSERTE(pszServiceName != NULL);

	SC_HANDLE hService;

	// open service handle with SERVICE_STOP and SERVICE_QUERY_STATUS access
	hService = OpenService(hSCM, pszServiceName, 
						   SERVICE_STOP|SERVICE_QUERY_STATUS);
	if (hService == NULL)
		return PrintError(_T("OpenService failed"), GetLastError());

	// send SERVICE_CONTROL_STOP command to the service and wait until it
	// reaches SERVICE_STOPPED state
	if (!ControlServiceAndWait(hService, SERVICE_CONTROL_STOP, 
							   SERVICE_STOPPED, 120000))
	{
		PrintError(_T("ControlServiceAndWait failed"), GetLastError());

		//_VERIFY(
			CloseServiceHandle(hService);//);
		return 1;
	}

	//_VERIFY(
		CloseServiceHandle(hService);//);
	return 0;
}

//---------------------------------------------------------------------------
// ControlServiceAndWait
//
//  Sends a control request to a service and waits until the service reaches
//	the specified state.
//
//  Parameters:
//	  hService  - service handle
//	  dwControl - control code
//	  dwState	- service state to wait for
//	  dwTimeout - timeout in milliseconds
//
//  Returns:
//	  TRUE, if successful, FALSE - otherwise.
//
BOOL
ControlServiceAndWait(
	IN SC_HANDLE hService,
	IN DWORD dwControl,
	IN DWORD dwState,
	IN DWORD dwTimeout
	)
{
	//_ASSERTE(hService != NULL);

	SERVICE_STATUS Status;
	DWORD dwStart;
	DWORD dwCheckPoint = (DWORD)-1;
	DWORD dwWait;

	// send control code
	if (!ControlService(hService, dwControl, &Status))
		return FALSE;

	// remember when the operation was started
	dwStart = GetTickCount();

	// wait until the service reaches the specified state
	while (Status.dwCurrentState != dwState &&
		   Status.dwCurrentState != SERVICE_STOPPED)
	{
		// check if timeout has expired
		if (dwTimeout != INFINITE)
		{
			if (GetTickCount() - dwStart >= dwTimeout)
				return SetLastError(ERROR_TIMEOUT), FALSE;
		}

		// determine how long to wait
		if (dwCheckPoint != Status.dwCheckPoint)
		{
			dwCheckPoint = Status.dwCheckPoint;
			dwWait = Status.dwWaitHint;
		}
		else
		{
			dwWait = 1000;
		}

		// do wait
		Sleep(dwWait);

		// query service status
		if (!QueryServiceStatus(hService, &Status))
			return FALSE;
	}

	if (Status.dwCurrentState == SERVICE_STOPPED &&
		Status.dwWin32ExitCode != ERROR_SUCCESS)
	{
		SetLastError(Status.dwWin32ExitCode);
		return FALSE;
	}

	return TRUE;
}