#pragma once
#include "Utils.h"
#include <cmath>

namespace mathUtils
{
	int** DoubleToInt(double** matrix, int rows, int columns);
	double** matrixMultiply(double value, double** matrix, int rows, int columns);
	double** matrixMultiply(double value, int** matrix, int rows, int columns);
	double** IntToDouble(int** matrix, int rows, int columns);
	double** arctan(double** matrix,double** matrix2, int rows, int columns) ;
	double** sinus(double** matrix, int rows, int columns);
	double** cosinus(double** matrix, int rows, int columns);
	inline double euclideanDist(double x1, double y1, double z1, double x2, double y2, double z2)
	{
		return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
	}
};
