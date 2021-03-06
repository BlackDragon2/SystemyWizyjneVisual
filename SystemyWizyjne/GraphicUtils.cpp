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
			if(B[i][j]==0)
				result[i][j]=0;
			else
				result[i][j]=(A[i][j]/(B[i][j]));
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
}

double** graphicUtils::toGrayScale(double** matrix, int rows, int columns, GrayMethod method)
{
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0;i<rows;i++)
		for(int j=0;j<columns;j++)
			result[i][j]=toGray(method, graphicIO::getR(matrix[i][j]), graphicIO::getB(matrix[i][j]), graphicIO::getB(matrix[i][j]));
	return result;
}

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
			result[i][j]=(int)(matrix[i][j]*255/min);
	return result;
}

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
			result[i][j]=(int)((double)matrix[i][j]*255/min);
	return result;
}

double** graphicUtils::opening(double** matrix, int rows, int columns, int maskRows, int maskColumns)
{
	double** temp=erosion(matrix, rows, columns, maskRows, maskColumns);
	double** result=dilation(matrix, rows, columns, maskRows, maskColumns);
	utils::memFree(temp, rows);
	return result;
}

int** graphicUtils::opening(int** matrix, int rows, int columns, int maskRows, int maskColumns)
{
	int** temp=erosion(matrix, rows, columns, maskRows, maskColumns);
	int** result=dilation(matrix, rows, columns, maskRows, maskColumns);
	utils::memFree(temp, rows);
	return result;
}

double** graphicUtils::closing(double** matrix, int rows, int columns, int maskRows, int maskColumns)
{
	double** temp=dilation(matrix, rows, columns, maskRows, maskColumns);
	double** result=erosion(matrix, rows, columns, maskRows, maskColumns);
	utils::memFree(temp, rows);
	return result;
}

int** graphicUtils::closing(int** matrix, int rows, int columns, int maskRows, int maskColumns)
{
	int** temp=dilation(matrix, rows, columns, maskRows, maskColumns);
	int** result=erosion(matrix, rows, columns, maskRows, maskColumns);
	utils::memFree(temp, rows);
	return result;
}

double** graphicUtils::dilation(double** matrix, int rows, int columns, int maskRows, int maskColumns)
{
	double** result=utils::create2DDouble(rows, columns);
	int w=(maskRows-1)/2;
	int h=(maskColumns-1)/2;
	int flag=1;
	for(int x=0;x<rows;x++)
	{
		for(int y=0;y<columns;y++)
		{
			flag=1;
			result[x][y]=0;
			for(int i=-w;i<=w&&flag;i++)
			{
				for(int j=-h;j<=h&&flag;j++)
				{
					if(x-i>=0&&y-j>=0&&x-i<rows&&y-j<columns)
						if(matrix[x-i][y-j]>0)
						{
							flag=0;
							result[x][y]=matrix[i][j];
						}
				}
			}			
		}
	}
	return result;
}

int** graphicUtils::dilation(int** matrix, int rows, int columns, int maskRows, int maskColumns)
{
	int** result=utils::create2DInt(rows, columns);
	int w=(maskRows-1)/2;
	int h=(maskColumns-1)/2;
	int flag=1;
	for(int x=0;x<rows;x++)
	{
		for(int y=0;y<columns;y++)
		{
			flag=1;
			result[x][y]=0;
			for(int i=-w;i<=w&&flag;i++)
			{
				for(int j=-h;j<=h&&flag;j++)
				{
					if(x-i>=0&&y-j>=0&&x-i<rows&&y-j<columns)
						if(matrix[x-i][y-j]>0)
						{
							flag=0;
							result[x][y]=matrix[i][j];
						}
				}
			}			
		}
	}
	return result;
}

double** graphicUtils::erosion(double** matrix, int rows, int columns, int maskRows, int maskColumns)
{
	double** result=utils::create2DDouble(rows, columns);
	int w=(maskRows-1)/2;
	int h=(maskColumns-1)/2;
	int flag=1;
	for(int x=0;x<rows;x++)
	{
		for(int y=0;y<columns;y++)
		{
			flag=1;
			result[x][y]=matrix[x][y];
			for(int i=-w;i<=w&&flag;i++)
			{
				for(int j=-h;j<=h&&flag;j++)
				{
					if(x-i>=0&&y-j>=0&&x-i<rows&&y-j<columns)
						if(matrix[x-i][y-j]==0)
						{
							flag=0;
							result[x][y]=0;
						}
				}
			}			
		}
	}
	return result;
}

int** graphicUtils::erosion(int** matrix, int rows, int columns, int maskRows, int maskColumns)
{
	int** result=utils::create2DInt(rows, columns);
	int w=(maskRows-1)/2;
	int h=(maskColumns-1)/2;
	int flag=1;
	for(int x=0;x<rows;x++)
	{
		for(int y=0;y<columns;y++)
		{
			flag=1;
			result[x][y]=matrix[x][y];
			for(int i=-w;i<=w&&flag;i++)
			{
				for(int j=-h;j<=h&&flag;j++)
				{
					if(x-i>=0&&y-j>=0&&x-i<rows&&y-j<columns)
						if(matrix[x-i][y-j]==0)
						{
							flag=0;
							result[x][y]=0;
						}
				}
			}			
		}
	}
	return result;
}
