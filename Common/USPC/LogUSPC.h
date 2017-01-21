#pragma once

class LogUSPC
{
    int offs;
public:
	char buffer[128][128];
	static void Clear();
	void Open();
    char *operator[](int);
};