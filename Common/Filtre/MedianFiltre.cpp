#include "stdafx.h"
#include "Filtre\MedianFiltre.h"
#include <windows.h>

int MedianFiltre::Init(int width_, double *d)
{
	if(width_ > 2 && width_ < 16)
	{
	    width_ |= 1;

		index = width_;

		medianIndex = width_ / 2;
		width = width_;
		ZeroMemory(ind, sizeof(ind));
		memcpy(buf, d, sizeof(double) * width_);
		for(int i = 0; i < width_ - 1; ++i)
		{
		   double x = buf[i];
		   for(int j = 1 + i; j < width_; ++j)
		   {
			   if(x > buf[j]) ++ind[i]; else ++ind[j];
		   }
		}

		for(int i = 0; i < width_; ++i)
			if(medianIndex == ind[i]) return i;
	}
	return 0;
}

int MedianFiltre::Add(double d)
{
	int index_ = index % width;
	++index;
	double x = buf[index_];
	for(int i = 0; i < width; ++i)
	{
		if(x < buf[i]) --ind[i];
	}

	int cnt = 0;
	buf[index_] = d;

	for(int i = 0; i < width; ++i)
	{
		if(i != index_)
		{
			if(d > buf[i]) ++cnt; else ++ind[i];
		}
	}
	ind[index_] = cnt;
	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return i;
	}
	return 0;
}
//----------------------------------------------------------------------
int MedianFiltre::Add(double d, int st)
{
	int index_ = index % width;
	++index;
	double x = buf[index_];
	for(int i = 0; i < width; ++i)
	{
		if(x < buf[i]) --ind[i];
	}

	int cnt = 0;
	buf[index_] = d;
	status[index_] = st;

	for(int i = 0; i < width; ++i)
	{
		if(i != index_)
		{
			if(d > buf[i]) ++cnt; else ++ind[i];
		}
	}
	ind[index_] = cnt;
	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return i;
	}
	return 0;
}
int MedianFiltre::Add(double d, double bit, int st)
{
	int index_ = index % width;
	++index;
	double x = buf[index_];
	for(int i = 0; i < width; ++i)
	{
		if(x < buf[i]) --ind[i];
	}

	int cnt = 0;
	buf[index_] = d;
	status[index_] = st;
	bit2[index_] = bit;

	for(int i = 0; i < width; ++i)
	{
		if(i != index_)
		{
			if(d > buf[i]) ++cnt; else ++ind[i];
		}
	}
	ind[index_] = cnt;
	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return i;
	}
	return 0;
}
int MedianFiltre::Add(double d, int st, void *p)
{
	int index_ = index % width;
	++index;
	double x = buf[index_];
	for(int i = 0; i < width; ++i)
	{
		if(x < buf[i]) --ind[i];
	}

	int cnt = 0;
	buf[index_] = d;
	status[index_] = st;
	data[index_] = p;

	for(int i = 0; i < width; ++i)
	{
		if(i != index_)
		{
			if(d > buf[i]) ++cnt; else ++ind[i];
		}
	}
	ind[index_] = cnt;
	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return i;
	}
	return 0;
}
int MedianFiltre::Add(double d, double b, int st, void *p)
{
	int index_ = index % width;
	++index;
	double x = buf[index_];
	for(int i = 0; i < width; ++i)
	{
		if(x < buf[i]) --ind[i];
	}

	int cnt = 0;
	buf[index_] = d;
	status[index_] = st;
	data[index_] = p;
	bit2[index_] = b;

	for(int i = 0; i < width; ++i)
	{
		if(i != index_)
		{
			if(d > buf[i]) ++cnt; else ++ind[i];
		}
	}
	ind[index_] = cnt;
	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == ind[i]) return i;
	}
	return 0;
}
//--------------------------------------------------------------------
void MedianFiltre::Clear(int width_)
{
	medianIndex = width_ / 2;
	index = 0;
	indexX = 0;
	width = width_;
	ZeroMemory(buf, sizeof(buf));
	ZeroMemory(bufX, sizeof(bufX));
	for(int i = 0; i < width; ++i)
	{
			ind[i] = i;
			indX[i] = i;
	}
}
//------------------------------------------------------------------
void MedianFiltre::SetVal(int width_, double val)
{
	medianIndex = width_ / 2;
	index = 0;
	width = width_;
	ZeroMemory(buf, sizeof(buf));
	for(int i = 0; i < width; ++i)
	{
		ind[i] = i;
		buf[i] = val;
	}
}
//------------------------------------------------------------------------------
int MedianFiltre::AddX(double d)
{
	int index_ = indexX % width;
	++indexX;
	double x = bufX[index_];
	for(int i = 0; i < width; ++i)
	{
		if(x < bufX[i]) --indX[i];
	}

	int cnt = 0;
	bufX[index_] = d;

	for(int i = 0; i < width; ++i)
	{
		if(i != index_)
		{
			if(d > bufX[i]) ++cnt; else ++indX[i];
		}
	}
	indX[index_] = cnt;
	for(int i = 0; i < width; ++i)
	{
		if(medianIndex == indX[i]) return i;
	}
	return 0;
}
//-------------------------------------------------------------------------