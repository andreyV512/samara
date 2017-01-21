// service.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <wbemidl.h>
#include <stdio.h>
#include "AsyncServer.h"
#include "InitTcp.h"


int _tmain(int argc, _TCHAR* argv[])
{
	InitTcp initTcp;
	AsyncServer::Create(8989);
	getchar();
	return 0;
}

