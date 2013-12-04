#pragma once
#include "Utils.h"
#include <cmath>

namespace mathUtils
{
	int** DoubleToInt(double** matrix, int rows, int columns);
	double** matrixMultiply(double value, double** matrix, int rows, int columns);
	double** matrixMultiply(double value, int** matrix, int rows, int columns);
	double** IntToDouble(int** matrix, int rows, int columns);
	double** arctan(double** matrix, int rows, int columns) ;
	double** sinus(double** matrix, int rows, int columns);
	double** cosinus(double** matrix, int rows, int columns);
};
