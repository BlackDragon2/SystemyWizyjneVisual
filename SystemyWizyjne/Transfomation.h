#pragma once
#include <cmath>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

namespace transformation
{
	void moveToStart(std::vector<double> in);//moves object so that is
	void transform(std::vector<double> in, double xAngle, double yAngle=0.0, double zAngle=0.0);
	void transform(double* x, double* y, double* z, double xAngle, double yAngle, double zAngle);
	inline double toRads(double degrees)
	{
		return degrees * (3.14159265/180.0); 
	};
	inline double toDegrees(double rads)
	{
		return  rads * (180.0/3.14159265);
	};
};

