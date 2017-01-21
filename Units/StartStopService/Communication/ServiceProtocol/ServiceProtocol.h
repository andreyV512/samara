#pragma once

#include "CommonTCP.h"

namespace StatusProtocol
{
	class Server: public CommonServer<Server>
	{
	public:
		static bool Do(Frame *);
	};
	class Client: public CommonClient
	{
	public:
		static void Do(wchar_t *addr, int port);
	};
}

namespace RunProtocol
{
	class Server: public CommonServer<Server>
	{
	public:
		static bool Do(Frame *);
	};
	class Client: public CommonClient
	{
	public:
		static void Do(wchar_t *addr, int port);
	};
}

namespace StopProtocol
{
	class Server: public CommonServer<Server>
	{
	public:
		static bool Do(Frame *);
	};
	class Client: public CommonClient
	{
	public:
		static void Do(wchar_t *addr, int port);
	};
}