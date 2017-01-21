#include "stdafx.h"
#include "Graphics/Chart.h"
#include "EmptyLeftAxes.h"

using namespace Gdiplus;
EmptyLeftAxes::EmptyLeftAxes(Chart &chart) 
 : chart(chart)
 , leftOffset(25)
{}
//-----------------------------------------------------------------------------------
void EmptyLeftAxes::Draw()
{
	//Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	//SolidBrush fontColor(chart.colorAxes);
//	StringFormat format;
	//format.SetAlignment(StringAlignmentNear);
	//double height;

	//int maxLen = 0;
	chart.offsetAxesLeft = leftOffset;//7 + chart.GetCountDigit(chart.minAxesY, chart.maxAxesY, height, font, maxLen);

	//chart.GetCountDigit(chart.minAxesY, chart.maxAxesY, height, font, maxLen);
	//maxLen += 2;
	int x = leftOffset;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, x, chart.rect.top + chart.offsetAxesTop, x, bottom);
	/*
	//char buf[32];
	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
	double deltaTick = 0;
	double deltaDigit = 0;
	double digit = 0;
	double minTick = 0;
	
	deltaDigit = 1;
	digit = chart.minAxesY;
	minTick = 0;
	deltaTick = (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
	double offs = chart.offsetGridY = chart.rect.bottom - chart.offsetAxesBottom + minTick;
	chart.deltaTickY = deltaTick;
	chart.deltaDigitY = deltaDigit;
	chart.offsetGridX = leftOffset + deltaTick;
	chart.offsetAxesLeft = leftOffset;// + deltaTick;
	while(bottom < offs)
	{
		offs -= deltaTick;
		digit += deltaDigit;
	}
	origin.X = (REAL)chart.rect.left;
	int len;
	while(offs > chart.rect.top + chart.offsetAxesTop)
	{
		chart.g->DrawLine(&pen, (REAL)x - 5, (REAL)offs, (REAL)x, (REAL)offs);
		//gcvt(digit, 5, buf);
		//mbstowcs(wbuf, buf, 32);
		_itow((int)digit, wbuf, 10);
		len = wcslen(wbuf) + 1;
		if(len <= maxLen)
		{
			chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
			origin.Y = REAL(offs - deltaTick + deltaTick/2);
			origin.X = leftOffset - rect.Width - 5;
			chart.g->DrawString(wbuf, len, &font, origin, &fontColor);
		}
		offs -= deltaTick;
		digit += deltaDigit;
	}	
	*/
}