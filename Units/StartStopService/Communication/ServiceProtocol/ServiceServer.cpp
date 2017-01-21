#include "stdafx.h"
#include "ServiceProtocol.h"
#include "Frame.h"
#include "NetFrame.h"
#include "Config.h"
#ifndef CLIENT
#include "Services.h"

namespace StatusProtocol
{
	bool Server::Do(Frame *f)
	{
		char *s = (char *)f->data.buf;
		Service::Status(s);
		int len = 1 + strlen(s);
		f->length = len;
		f->proc = Stop;
		return true;
	}
}

namespace RunProtocol
{
	bool Server::Do(Frame *f)
	{
		Service::Start();
		f->length = 0;
		f->proc = Stop;
		return true;
	}
}

namespace StopProtocol
{
	bool Server::Do(Frame *f)
	{
		Service::Stop();
		f->length = 0;
		f->proc = Stop;
		return true;
	}
}
#else
namespace StatusProtocol
{
	bool Server::Do(Frame *f)
	{
		return true;
	}
}

namespace RunProtocol
{
	bool Server::Do(Frame *f)
	{
		return true;
	}
}

namespace StopProtocol
{
	bool Server::Do(Frame *f)
	{
		return true;
	}
}
#endif