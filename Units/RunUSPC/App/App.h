#pragma once

class App
{
public:
	bool Init();
	void Destroy();
};

template<class T>struct Singleton
{
	static T& Instance(){static T x; return x;}
};
