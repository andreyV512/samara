#pragma once
#include "window_tool/message.h"
class TEvent
{
public:
	virtual void Do(TCommand &){}
	virtual void Do(TNotify &){}
};