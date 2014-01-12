#include "GraphicIO.h"

using namespace std;
using namespace cv;

Mat graphicIO::createImage(int** matrix, int rows, int columns, int flag)
{
	Mat image(Size(columns,rows),CV_8UC3);
	if(!flag)
	{
		for(int i=0; i<image.rows; i++)
		{
			for(int j=0; j<image.cols; j++)
			{
				image.data[image.step[0]*i + image.step[1]* j + 0] = getB(matrix[i][j]);
				image.data[image.step[0]*i + image.step[1]* j + 1] = getG(matrix[i][j]);
				image.data[image.step[0]*i + image.step[1]* j + 2] = getR(matrix[i][j]);
			}
		}
	}
	else
	{
		for(int i=0; i<image.rows; i++)
		{
			for(int j=0; j<image.cols; j++)
			{
				image.data[image.step[0]*i + image.step[1]* j + 0] = matrix[i][j];
				image.data[image.step[0]*i + image.step[1]* j + 1] = matrix[i][j];
				image.data[image.step[0]*i + image.step[1]* j + 2] = matrix[i][j];
			}
		}
	}
	return image;
};

Mat graphicIO::createImage(int** matrix, int rows, int columns, Position position)
{
		Mat result;
		if(position==HORIZONTAL)
		{
			result= Mat(Size(columns,rows),CV_8UC3);
			for(int i=0; i<result.rows; i++)
			{
				for(int j=0; j<result.cols; j++)
				{
					result.data[result.step[0]*i + result.step[1]* j + 0] = getB(matrix[j][i]);
					result.data[result.step[0]*i + result.step[1]* j + 1] = getG(matrix[j][i]);
					result.data[result.step[0]*i + result.step[1]* j + 2] = getR(matrix[j][i]);
				}
			} 
		}
		else
		{
			result= Mat(Size(columns,rows),CV_8UC3);
			for(int i=0; i<result.rows; i++)
			{
				for(int j=0; j<result.cols; j++)
				{
					result.data[result.step[0]*i + result.step[1]* j + 0] = getB(matrix[i][j]);
					result.data[result.step[0]*i + result.step[1]* j + 1] = getG(matrix[i][j]);
					result.data[result.step[0]*i + result.step[1]* j + 2] = getR(matrix[i][j]);
				}
			} 
		}
		return result;
};

Mat graphicIO::createImage(double** matrix, int rows, int columns, int flag)
{
	Mat image(Size(columns,rows),CV_8UC3);
	if(!flag)
	{
		for(int i=0; i<image.rows; i++)
		{
			for(int j=0; j<image.cols; j++)
			{
				image.data[image.step[0]*i + image.step[1]* j + 0] = getB((int)matrix[i][j]);
				image.data[image.step[0]*i + image.step[1]* j + 1] = getG((int)matrix[i][j]);
				image.data[image.step[0]*i + image.step[1]* j + 2] = getR((int)matrix[i][j]);
			}
		}
	}
	else
	{
		for(int i=0; i<image.rows; i++)
		{
			for(int j=0; j<image.cols; j++)
			{
				image.data[image.step[0]*i + image.step[1]* j + 0] = (int)matrix[i][j];
				image.data[image.step[0]*i + image.step[1]* j + 1] = (int)matrix[i][j];
				image.data[image.step[0]*i + image.step[1]* j + 2] = (int)matrix[i][j];
			}
		}
	}
	return image;
};

Mat graphicIO::createImage(double** matrix, int rows, int columns, Position position)
{
		Mat result;
		if(position==HORIZONTAL)
		{
			result= Mat(Size(columns,rows),CV_8UC3);
			for(int i=0; i<result.rows; i++)
			{
				for(int j=0; j<result.cols; j++)
				{
					result.data[result.step[0]*i + result.step[1]* j + 0] = getB((int)matrix[j][i]);
					result.data[result.step[0]*i + result.step[1]* j + 1] = getG((int)matrix[j][i]);
					result.data[result.step[0]*i + result.step[1]* j + 2] = getR((int)matrix[j][i]);
				}
			} 
		}
		else
		{
			result= Mat(Size(columns,rows),CV_8UC3);
			for(int i=0; i<result.rows; i++)
			{
				for(int j=0; j<result.cols; j++)
				{
					result.data[result.step[0]*i + result.step[1]* j + 0] = getB((int)matrix[i][j]);
					result.data[result.step[0]*i + result.step[1]* j + 1] = getG((int)matrix[i][j]);
					result.data[result.step[0]*i + result.step[1]* j + 2] = getR((int)matrix[i][j]);
				}
			} 
		}
		return result;
};

Mat graphicIO::createGreyscaleImage(int** matrix, int rows, int columns, GrayMethod method)
{
	Mat result(Size(columns,rows),CV_8UC3);
	for(int i=0; i<result.rows; i++)
	{
		for(int j=0; j<result.cols; j++)
		{
			int value=toGray(method, getR(matrix[i][j]), getB(matrix[i][j]), getG(matrix[i][j]));
			result.data[result.step[0]*i + result.step[1]* j + 0] = value;
			result.data[result.step[0]*i + result.step[1]* j + 1] = value;
			result.data[result.step[0]*i + result.step[1]* j + 2] = value;
		}
	}
	return result;
};

double** graphicIO::getImageInArray(string path, int &rows, int &columns)
{
	return fileIO::loadArray(path, rows, columns);
};

double** graphicIO::getImageInArray(const char* path, int &rows, int &columns)
{
	return fileIO::loadArray(path, rows, columns);
};

double** graphicIO::getImageInArray(cv::Mat image, int &rows, int &columns)
{
	columns=image.cols;
	rows=image.rows;
	double** result=utils::create2DDouble(rows, columns);
	for(int i=0; i<image.rows; i++)
	{
		for(int j=0; j<image.cols; j++)
		{result[i][j]=image.data[image.step[0]*i + image.step[1]* j + 0];
			//result[i][j]=image.data[image.step[0]*i + image.step[1]* j + 0]<<16;
			//result[i][j]+=image.data[image.step[0]*i + image.step[1]* j + 1]<<8;
			//result[i][j]+=image.data[image.step[0]*i + image.step[1]* j + 2];	
		}
	}
	return result;
};

Mat	graphicIO::scaleImage(Mat inputImage, int outputrows, int outputcolumns)
{
	resize(inputImage, inputImage, Size(outputcolumns, outputrows));
	return inputImage;
};

int graphicIO::RGBFromGRAY(int value)
{
	int result = 0;
	result+= (value<< 16);
	result+= (value<< 8);
	result+=  value;
	return result;
}

void graphicIO::showImage(double** array, int rows, int columns, int flag)
{
	cv::Mat image=createImage(array, rows, columns, flag);
	showImage(image);
}

void graphicIO::showImage(int** array, int rows, int columns, int flag)
{
	cv::Mat image=createImage(array, rows, columns, flag);
	showImage(image);
}


