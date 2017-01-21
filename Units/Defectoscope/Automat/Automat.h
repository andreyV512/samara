#pragma once

class Automat
{
public:
	struct Impl;
private:
	Impl &impl;
public:
	Automat();
	void Init();
	static void Stop();
};

extern Automat automat;

bool USPC_Test();

