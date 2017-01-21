#pragma once

class GetHostByName
{
	wchar_t buf[128];
public:
	wchar_t *operator()(wchar_t *);
};