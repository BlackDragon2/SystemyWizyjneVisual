#include "GraphicUtils.h"

double** graphicUtils::GaussianKernel(double phi, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	double power;
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<columns;j++)
		{
			power=(pow((-rows)/2+i,2)+pow((-columns)/2+j,2))/(2*pow(phi, 2));
			result[i][j]=(1/(2*PI*phi*phi)*pow(E, -power));
		}
	}
	return result;
}

double** graphicUtils::convolution(double** mask, int maskRows, int maskColumns, double** matrix, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	int w=(maskRows-1)/2;
	int h=(maskColumns-1)/2;
	double sum=0.0;

	for(int x=0;x<rows;x++)
	{
		for(int y=0;y<columns;y++)
		{
			sum=0.0;
			for(int i=-w;i<=w;i++)
			{
				for(int j=-h;j<=h;j++)
				{
					if(x-i>=0&&y-j>=0&&x-i<rows&&y-j<columns)
						sum+=mask[i+w][j+h]*matrix[x-i][y-j];
				}
			}
			result[x][y]=sum;				
		}
	}
	return result;
};

double** graphicUtils::convolution(double** mask, int maskRows, int maskColumns, int** matrix, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	int w=(maskRows-1)/2;
	int h=(maskColumns-1)/2;
	double sum=0.0;

	for(int x=0;x<rows;x++)
	{
		for(int y=0;y<columns;y++)
		{
			sum=0.0;
			for(int i=-w;i<=w;i++)
			{
				for(int j=-h;j<=h;j++)
				{
					if(x-i>=0&&y-j>=0&&x-i<rows&&y-j<columns)
						sum+=mask[i+w][j+h]*matrix[x-i][y-j];
				}
			}
			result[x][y]=sum;				
		}
	}
	return result;
};

double** graphicUtils::normalize(double** matrix, int rows, int columns, bool positive)
{
	if(positive)
		return PositiveNormalize(matrix, rows, columns);
	else
		return NegativeNormalize(matrix, rows, columns);
};

double** graphicUtils::PositiveNormalize(double** matrix, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	double sum=0.0;

	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			sum+=matrix[i][j];
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=matrix[i][j]/sum;
	return result;
};

double** graphicUtils::NegativeNormalize(double** matrix, int rows, int columns)
{
		double maximum=matrix[0][0];
		double minimum=matrix[0][0];
		double** result=utils::create2DDouble(rows, columns);
		for(int i=0;i<rows;i++)
			for(int j=0;j<columns;j++)
			{
				maximum=max(maximum, matrix[i][j]);
				minimum=min(minimum, matrix[i][j]);
			}
		double dif=maximum-minimum;
		for(int i=0;i<rows;i++)
			for(int j=0;j<columns;j++)
				result[i][j]=(matrix[i][j]-minimum)/dif;
		return result;
};

double** graphicUtils::gradient(GradientMask mask, int** image, int rows, int columns)
{
	return convolution(getGradientMask(mask), 3, 3, image, rows, columns);
};

double** graphicUtils::gradient(GradientMask mask, double** image, int rows, int columns)
{
	return convolution(getGradientMask(mask), 3, 3, image, rows, columns);
};

double** graphicUtils::pixelAddN(double** A, double** B, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=min(255.0, A[i][j]+B[i][j]);
	return result;
};

double** graphicUtils::pixelAdd(double** A, double** B, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=A[i][j]+B[i][j];
	return result;
};

double** graphicUtils::pixelSubstractN(double** A, double** B, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=max(0.0, A[i][j]-B[i][j]);
	return result;
};

double** graphicUtils::pixelSubstract(double** A, double** B, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=A[i][j]-B[i][j];
	return result;
};

double** graphicUtils::pixelMultiplyN(double** A, double** B, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=(A[i][j]*B[i][j])/255.0;
	return result;
};

double** graphicUtils::pixelMultiply(double** A, double** B, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=A[i][j]*B[i][j];
	return result;
};

double** graphicUtils::pixelDivideN(double** A, double** B, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=(int)(A[i][j]/(B[i][j]+1));
	return result;
};

double** graphicUtils::pixelDivide(double** A, double** B, int rows, int columns)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=(A[i][j]/(B[i][j]+1));
	return result;
};

double** graphicUtils::pixelMultiplyN(double value, double** matrix, int rows, int columns)
{
	double** result=mathUtils::matrixMultiply(value, matrix, rows, columns);
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<columns;j++)
			result[i][j]=min(255.0, max(0.0, result[i][j]));
	}
	return result;
};

double** graphicUtils::pixelMultiplyN(double value, int** matrix, int rows, int columns)
{
	double** result=mathUtils::matrixMultiply(value, matrix, rows, columns);
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<columns;j++)
			result[i][j]=min(255.0, max(0.0, result[i][j]));
	}
	return result;
};

double** graphicUtils::pixelMultiply(double value, int** matrix, int rows, int columns)
{
	return mathUtils::matrixMultiply(value, matrix, rows, columns);
}

double** graphicUtils::pixelMultiply(double value, double** matrix, int rows, int columns)
{
	return mathUtils::matrixMultiply(value, matrix, rows, columns);
}

int** graphicUtils::toGrayScale(int** matrix, int rows, int columns, GrayMethod method)
{
	int** result=utils::create2DInt(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=toGray(method, graphicIO::getR(matrix[i][j]), graphicIO::getB(matrix[i][j]), graphicIO::getB(matrix[i][j]));
	return result;
};

double** graphicUtils::toGrayScale(double** matrix, int rows, int columns, GrayMethod method)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=toGray(method, graphicIO::getR(matrix[i][j]), graphicIO::getB(matrix[i][j]), graphicIO::getB(matrix[i][j]));
	return result;
};

int** graphicUtils::range(double** matrix, int rows, int columns)
{
	int** result=utils::create2DInt(rows, columns);
	double min=matrix[0][0];
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			if(min>matrix[i][j])
				min=matrix[i][j];
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=(int)(matrix[i][j]*255/min);cout<<min<<endl;
	return result;
};

int** graphicUtils::range(int** matrix, int rows, int columns)
{
		int** result=utils::create2DInt(rows, columns);
	double min=matrix[0][0];
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			if(min>matrix[i][j])
				min=matrix[i][j];
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=(int)((double)matrix[i][j]*255/min);cout<<min<<endl;
	return result;
};
