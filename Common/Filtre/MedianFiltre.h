#pragma once 

struct MedianFiltre
{
	int index;
	int width;
	int medianIndex;
	double buf[16];
	double bit2[16];
	int status[16];
	void *data[16];
	int ind[16];
	int Init(int , double *);
	int Add(double);
	int Add(double, int);
	int Add(double, double, int);
	int Add(double d, int st, void *p);
	int Add(double d, double b, int st, void *p);
	void Clear(int);
	void SetVal(int, double );
public:
	int indexX;
	double bufX[16];
	int indX[16];
	int AddX(double);
};