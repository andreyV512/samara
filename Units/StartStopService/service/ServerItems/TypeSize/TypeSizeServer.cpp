#include "stdafx.h"
#include "TypeSizeProtocol.h"
#include "../Frame.h"
#include "../NetFrame.h"
#include "DebugMess.h"

namespace TypeSizeProtocol
{
	bool Server::Do(Frame *f)
	{
		wchar_t *s = (wchar_t *)f->data.buf;
		
//		int len = 1 + wcslen(ret);
	//	wcscpy_s((wchar_t *)f->data.buf, len, ret);		
		f->length = 0;//2 * len;
		f->proc = Stop;
		return true;
	}
}