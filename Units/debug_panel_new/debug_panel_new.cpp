// debug_panel_new.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <clocale>
#include "debug_tools/DebugMess.h"


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "rus"); 
	ViewerDebugMess x;
	printf("������ ��� ������ ���������� ����������\n");
	for(;;)
	{
		char *b = x.get();
		if(NULL != b) printf(b);
	}
	return 0;
}

