#include "stdafx.h"
#include "Frame.h"
#include "debug_tools/DebugMess.h"

CRITICAL_SECTION cs;
Frame *head = NULL;

namespace
{
	struct Init
	{
		Init()
		{
			InitializeCriticalSection(&cs);
		}
	};
	volatile Init init;
}

Frame::Frame()
{}
static int counter = 0;
static int drop = 0;
static int nex = 0;
Frame *Frame::_Get()
{
	Frame *result = NULL;
	EnterCriticalSection(&cs);
	result = head;
	if (NULL != result) 
	{
			head = head->next;
	}
	else
	{
		result = new Frame;
		result->next = NULL;
	}
	LeaveCriticalSection(&cs);
	return result;
}

void Frame::Drop(void *data)
{
	EnterCriticalSection(&cs);
	((Frame *)data)->next = head;
	head = (Frame *)data;
	--nex;
	LeaveCriticalSection(&cs);
}