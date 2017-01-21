#pragma once
#include "ThresholdWindow.h"

class LongThresholdWindow: public TresholdWindow
{
	double &minAxesY;
	double &maxAxesY;
public:
	typedef TresholdWindow Parent;
	LongThresholdWindow();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TClose &);
	static wchar_t *Title();
};