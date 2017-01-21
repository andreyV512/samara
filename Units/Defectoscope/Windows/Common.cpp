#include "stdafx.h"
#include "Common.h"

#include "ThicknessWindow.h"
#include "LongWindow.h"
#include "CrossWindow.h"
#include "DiffWindows/MainWindow.h"

void CloseAllWindows()
{
	Common::DestroyWindow<ThicknessWindow>();
	Common::DestroyWindow<LongWindow     >();
	Common::DestroyWindow<CrossWindow    >();
	Common::DestroyWindow<ScanWindow     >();
}

void ZoneToCoord(Chart &chart, int zone, int sens, WORD &x, WORD &y)
{
	double dX = (chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight)
				/(chart.maxAxesX - chart.minAxesX);
	x = (WORD)(chart.rect.left + chart.offsetAxesLeft + dX * zone + dX / 2);

	double dY = (chart.rect.bottom - chart.rect.top - chart.offsetAxesTop - chart.offsetAxesBottom)
				/(chart.maxAxesY - chart.minAxesY);
	y = (WORD)(chart.rect.bottom - (chart.offsetAxesBottom + dY * sens + dY / 2));
}

namespace Common
{
	template<class T>void __set_param__(HWND h)
	{
		T &t = *(T *)GetWindowLongPtr(h, GWLP_USERDATA);
		typedef typename T::viewers_list::Head TViewer;
		typedef typename TL::Inner<TViewer>::Result TMainViewer;
		TViewer &viewer = t.viewers.get<TViewer>();
		TMainViewer &mainViewer = app.mainWindow.viewers.get<TMainViewer>();
		viewer.mouseMove = false;
		viewer.currentX = mainViewer.currentX;
		viewer.currentY = mainViewer.currentY;	

		ZoneToCoord(viewer.chart, viewer.currentX, viewer.currentY
			, viewer.storedMouseMove.x, viewer.storedMouseMove.y);
		
		SendMessage(h, WM_PAINT, 0, 0);
	}
	
	void SetParamOpenWindow<CrossWindow>::operator()(HWND h)
	{
        __set_param__<CrossWindow>(h);
	}

	void SetParamOpenWindow<ThicknessWindow>::operator()(HWND h)
	{
		__set_param__<ThicknessWindow>(h);
	}
	void SetParamOpenWindow<LongWindow>::operator()(HWND h)
	{
		__set_param__<LongWindow>(h);
	}
}