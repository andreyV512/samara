#pragma once
#include <GdiPlus.h>
#include "window_tool/message.h"
class IOportsViewer
{
	HANDLE hTimer;	
public:
	HWND &hWnd;
	Gdiplus::Bitmap *&backScreen;
	bool lastTitle;
public:
	IOportsViewer(HWND &, Gdiplus::Bitmap *&);
	void Size(Gdiplus::Graphics &, int, int);
	void Start();
	void Stop();
	void MouseLDown(TLButtonDown &);
};