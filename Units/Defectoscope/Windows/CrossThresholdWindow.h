#pragma once
#include "ThresholdWindow.h"

class CrossThresholdWindow: public TresholdWindow
{
	double &minAxesY;
	double &maxAxesY;
public:
	typedef TresholdWindow Parent;
	CrossThresholdWindow();
	unsigned operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TClose &);
	static wchar_t *Title();
};