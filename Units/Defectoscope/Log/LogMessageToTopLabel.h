#pragma once

class LogMessageToTopLabel
{
	HANDLE hTimer;
	void Do();
	static VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
public:
	LogMessageToTopLabel();
	~LogMessageToTopLabel();
};