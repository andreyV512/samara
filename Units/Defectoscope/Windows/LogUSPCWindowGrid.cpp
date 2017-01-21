#include "stdafx.h"
#include "LogUSPCWindowGrid.h"
#include "debug_tools/DebugMess.h"
#include "Log\LogBuffer.h"
#include "LogUSPCWindow.h"
//------------------------------------------------------------------------
extern HINSTANCE hInstance;
//------------------------------------------------------------------------
template<class T>struct header_table;
#define Z(T, w, txt)\
struct T{};\
template<>struct header_table<T>\
{\
	LPWSTR name(){return L##txt;}\
	static const int width = w;\
};

Z(Date, 140, "Дата")
Z(Type, 70, "Событие")
Z(Color, 700, "Сообщение")
#undef Z

typedef TL::MkTlst<Date, Type, Color>::Result ParameterNameList;
//--------------------------------------------------------------------------------
LogUSPCWindowGrid::LogUSPCWindowGrid()
{
}
//--------------------------------------------------------------------------------
void LogUSPCWindowGrid::Init(HWND h)
{
	hWnd = handlers.Init<ParameterNameList>(h, 128);
}
//--------------------------------------------------------------------
#pragma warning(disable : 4995)
void LogUSPCWindowGrid::Handlers::operator()(TCellData &l)
{
#pragma warning(disable : 4996)
	if(l.row < 128)
	{
		char *s = ((LogUSPCWindow *)GetWindowLongPtr(l.h, GWLP_USERDATA))->data[l.row];
		if(0 == s[0]) return;
		int z = 0;
		int i = 0;
		int len = (int)strlen(s);
		int start = 0;
		int count = 0;
		switch(l.col)
		{
			case 0:
				for(; i < len; ++i)
				{		
					if(' ' == s[i]) ++z;
					if(z > 1)
					{
						break;
					}
				}
                MultiByteToWideChar(1251, 0, s, i, l.data, i);
				l.data[i] = 0;
			break;
			case 1:
				for(; i < len; ++i)
				{		
					if(' ' == s[i]) ++z;
					if(z > 1)
					{						
						break;
					}
				}
				for(; i < len; ++i)if(' ' != s[i])break;
				start = i;
				for(; i < len; ++i)
				{		
					if(' ' == s[i]) ++z;
					if(z > 2)
					{
						break;
					}
				}
				count = i - start;
                MultiByteToWideChar(1251, 0, &s[start], count, l.data, count);
				l.data[count] = 0;
			break;
			case 2:
				for(; i < len; ++i)
				{		
					if(' ' == s[i]) ++z;
					if(z > 2)
					{
						break;
					}
				}
				for(; i < len; ++i)if(' ' != s[i])break;	
				for(; i < len; ++i)
				{		
					if(' ' == s[i]) ++z;
					if(z > 3)
					{
						break;
					}
				}
				for(; i < len; ++i)if(' ' != s[i])break;
				count = len - i;
                MultiByteToWideChar(1251, 0, &s[i], count, l.data, count);
				l.data[count] = 0;
			break;
		}
	}
}
//--------------------------------------------------------------------
void LogUSPCWindowGrid::Handlers::operator()(TCellColor &l)
{	
	//Log::TData *d = NULL;
	//if(Log::IsRow(l.row, d))
	//{
	//	LogMess::FactoryMessages::Instance().Color(d->id, (unsigned &)l.bkColor, (unsigned &)l.textColor);
	//}
}
//---------------------------------------------------------------------------
