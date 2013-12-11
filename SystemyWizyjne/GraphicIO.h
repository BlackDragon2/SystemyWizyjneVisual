#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include "Position.h"
#include "GrayMethod.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "FileIO.h"
#include "Utils.h"

namespace graphicIO
{
	inline void saveImage(cv::Mat image, const char* path);
	inline void saveImage(cv::Mat image, std::string path);
	inline void saveImage(int** matrix, int rows, int columns, const char* path);
	inline void saveImage(int** matrix, int rows, int columns, std::string path);
	cv::Mat createImage(int** matrix, int rows, int columns, int flag=0);
	cv::Mat createImage(int** matrix, int rows, int columns, Position position);
	cv::Mat createImage(double** matrix, int rows, int columns, int flag=0);
	cv::Mat createImage(double** matrix, int rows, int columns, Position position);
	cv::Mat createGreyscaleImage(int** matrix, int rows, int columns, GrayMethod method);
	inline cv::Mat getImage(string path)
	{
		return cv::imread(path, CV_LOAD_IMAGE_COLOR);
	};

	inline cv::Mat	getImage(const char* path)
	{
		return cv::imread(path, CV_LOAD_IMAGE_COLOR);
	};
	double** getImageInArray(std::string path, int &rows, int &columns);
	double** getImageInArray(const char* path, int &rows, int &columns);
	double** getImageInArray(cv::Mat image, int &rows, int &columns);
	cv::Mat	scaleImage(cv::Mat inputImage, int outputrows, int outputcolumns);
	int RGBFromGRAY(int value);
	inline int getR(int color)
	{
		return color & 0xff;
	};

	inline int getG(int color)
	{
		return (color >> 8) & 0xff;
	};

	inline int getB(int color)
	{
		return (color >> 16) & 0xff;
	};

	inline void showImage(cv::Mat image)
	{
		cv::namedWindow("Image", CV_WINDOW_AUTOSIZE );
		imshow("Image", image);
		cv::waitKey(0);
	};

	void showImage(double** array, int rows, int columns, int flag);
	void showImage(int** array, int rows, int columns, int flag);
};

