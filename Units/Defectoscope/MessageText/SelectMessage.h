#pragma once
#include "DiffApp\App.h" 
char *SelectMessage(int *, int &);
//void SelectMessageBits(unsigned, int &);

struct StatusText
{
	char *operator()(int id, int &color, bool &visible);	
};

struct ColorBar
{
	void operator()(double &data, unsigned &color, int id, double defData);
};

bool TubeResult(char (&res)[App::count_zones], int count);



