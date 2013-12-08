#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <cstring>
#include <cstdlib>

using namespace std;

const double PI=3.14159265359;
const double E=2.71828182846;

namespace utils
{
	void memFree(double** array, int rows);
	void memFree(int** array, int rows);
	int** create2DInt(int rows, int columns);
	double** create2DDouble(int rows, int columns);
	vector<string> split(string line, string delimiter);
	double* parseDouble(vector<string> vect);
	int* parseInt(vector<string> vec);
	template<typename T>
	std::string to_string(T number)
	{
		std::ostringstream convert;
		convert << number;
		return convert.str();
	};
	template<typename T>
	void clear2D(T** array, int rows, int columns)
	{
		for(int i=0;i<rows;i++)
			for(int j=0;j<columns;j++)
				T[i][j]=0;
	}
};
