#pragma once
#include "Utils.h"
#include "MathUtils.h"
#include "GraphicIO.h"
#include "GradientMask.h"
#include "GrayMethod.h"

namespace graphicUtils
{
	double** GaussianKernel(double phi, int rows, int columns);
	double** convolution(double** mask, int maskRows, int maskColumns, double** matrix, int rows, int columns);
	double** convolution(double** mask, int maskRows, int maskColumns, int** matrix, int rows, int columns);
	double** opening(double** matrix, int rows, int columns, int maskRows, int maskColumns);
	int** opening(int** matrix, int rows, int columns, int maskRows, int maskColumns);
	double** closing(double** matrix, int rows, int columns, int maskRows, int maskColumns);
	int** closing(int** matrix, int rows, int columns, int maskRows, int maskColumns);
	double** dilation(double** matrix, int rows, int columns, int maskRows, int maskColumns);
	int** dilation(int** matrix, int rows, int columns, int maskRows, int maskColumns);
	double** erosion(double** matrix, int rows, int columns, int maskRows, int maskColumns);
	int** erosion(int** matrix, int rows, int columns, int maskRows, int maskColumns);
	double** normalize(double** matrix, int rows, int columns, bool positive);
	double** PositiveNormalize(double** matrix, int rows, int columns);
	double** NegativeNormalize(double** matrix, int rows, int columns);
	double** gradient(GradientMask mask, int** image, int rows, int columns);
	double** gradient(GradientMask mask, double** image, int rows, int columns);
	double** pixelAddN(double** A, double** B, int rows, int columns);
	double** pixelAdd(double** A, double** B, int rows, int columns);
	double** pixelSubstractN(double** A, double** B, int rows, int columns);
	double** pixelSubstract(double** A, double** B, int rows, int columns);
	double** pixelMultiplyN(double** A, double** B, int rows, int columns);
	double** pixelMultiply(double** A, double** B, int rows, int columns);
	double** pixelDivideN(double** A, double** B, int rows, int columns);
	double** pixelDivide(double** A, double** B, int rows, int columns);
	int** toGrayScale(int** matrix, int rows, int columns, GrayMethod method);
	double** toGrayScale(double** matrix, int rows, int columns, GrayMethod method);
	double** pixelMultiply(double value, double** matrix, int rows, int columns);
	double** pixelMultiply(double value, int** matrix, int rows, int columns);
	double** pixelMultiplyN(double value, double** matrix, int rows, int columns);
	double** pixelMultiplyN(double value, int** matrix, int rows, int columns);
	int** range(double** matrix, int rows, int columns);
	int** range(int** matrix, int rows, int columns);
};