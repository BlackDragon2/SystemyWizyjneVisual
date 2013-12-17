#include "MathUtils.h"

int** mathUtils::DoubleToInt(double** matrix, int rows, int columns)
{
	int** result=utils::create2DInt(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=(int)matrix[i][j];
	return result;
};

double** mathUtils::matrixMultiply(double value, double** matrix, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
	for(int j=0;j<columns;j++)
		result[i][j]=matrix[i][j]*value;
	return result;
};

double** mathUtils::matrixMultiply(double value, int** matrix, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
	for(int j=0;j<columns;j++)
		result[i][j]=matrix[i][j]*value;
	return result;
};

double** mathUtils::IntToDouble(int** matrix, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=matrix[i][j];
	return result;
};

double** mathUtils::arctan(double** matrix, double** matrix2, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=atan2(matrix[i][j], matrix2[i][j]);
	return result;
};

double** mathUtils::sinus(double** matrix, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
	for(int j=0;j<columns;j++)
		result[i][j]=sin(matrix[i][j]);
	return result;
};

double** mathUtils::cosinus(double** matrix, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
	for(int j=0;j<columns;j++)
		result[i][j]=cos(matrix[i][j]);
	return result;
};