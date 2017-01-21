#include "stdafx.h"
#include "LogMessageToTopLabel.h"
#include "DiffApp/App.h"
#include "LogMessages.h"
#include "Log\LogBuffer.h"

LogMessageToTopLabel::LogMessageToTopLabel()
	: hTimer(INVALID_HANDLE_VALUE)
{
	CreateTimerQueueTimer(&hTimer, NULL, WaitOrTimerCallback, this, 3000, 1000, WT_EXECUTEINTIMERTHREAD);
}

LogMessageToTopLabel::~LogMessageToTopLabel()
{
	DeleteTimerQueueTimer(NULL, hTimer, NULL);
}

namespace
{
	LogMess::FactoryMessages &f = LogMess::FactoryMessages::Instance();
}

void LogMessageToTopLabel::Do()
{
	Log::TData *d;
	if(Log::LastMessage(d))
	{		
		wchar_t mess[128];
		unsigned textColor, backColor;
		f.Color(d->id, backColor, textColor);
		char c[128];
		f.Text(d->id, c, d->value);
		backColor = ((backColor << 16) & 0xff0000) | ((backColor >> 16) & 0xff) | (backColor & 0xff00);
		wsprintf(mess, L"<%6x>%S", backColor, c);
		app.MainWindowTopLabel(mess);
	}
}

VOID CALLBACK LogMessageToTopLabel::WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	((LogMessageToTopLabel *)lpParameter)->Do();
}
