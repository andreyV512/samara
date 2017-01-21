#pragma once
#include "DiffApp/App.h"

template<class T>void StatusZoneDefect(int offs, double data, int zone, double (&brakThreshold)[App::count_zones], double (&klass2Threshold)[App::count_zones], char &status)
{
	 if(data > brakThreshold[zone])
	 {
		 status = StatusId<Clr<BorderDefect<T>>>();
	 }
	 else  if(data > klass2Threshold[zone])
	 {
		 status = StatusId<Clr<BorderKlass2<T>>>();
	 }
	 else
	 {
		status = StatusId<Clr<Nominal>>();
	 }
}


void StatusZoneThickness(int offs, double &data, int zone, double (&maxThreshold)[App::count_zones]
    , double (&minThreshold)[App::count_zones], double (&nominalTreshold)[App::count_zones], char &status);

class Compute
{
public:
	bool tubeResult;
	int lengthTube;
	Compute();
	void LengthTube(unsigned startTime, unsigned baseTime, unsigned stopTime);
	void CancelOperatorClear();
	void Recalculation();
};

extern Compute compute;