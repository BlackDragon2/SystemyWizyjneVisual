#include "Utils.h"


using namespace std;

void utils::memFree(double** array, int rows)
{
	for(int i=0;i<rows;i++)
		delete[] (array[i]);
	delete[] array;
};

void utils::memFree(int** array, int rows)
{
	for(int i=0;i<rows;i++)
		delete[] (array[i]);
	delete[] array;
};

int** utils::create2DInt(int rows, int columns)
{
	int** result= new int*[rows];
	for(int i=0;i<rows;i++)
		result[i]=new int[columns];
	return result;
};

double** utils::create2DDouble(int rows, int columns)
{
	double** result= new double*[rows];
	for(int i=0;i<rows;i++)
		result[i]=new double[columns];
	return result;
};

vector<string> utils::split(string line, string delimiter)
{
	istringstream buf(line);
    istream_iterator<string> iterator(buf), end;

    vector<string> tokens;
	size_t pos = 0;
	string token;
	while ((pos = line.find(delimiter)) != std::string::npos) 
	{
		token = line.substr(0, pos);
		tokens.push_back(token);
		line.erase(0, pos + delimiter.length());
	}
	return tokens;
};

double* utils::parseDouble(vector<string> vec)
{
	double* result=new double[vec.size()];
	for(int i=0;i<(int)vec.size();i++)
	{
		result[i]=atof(vec[i].c_str());
	}
	return result;
};

int* utils::parseInt(vector<string> vec)
{
	int* result=new int[vec.size()];
	for(int i=0;i<(int)vec.size();i++)
	{
		result[i]=atoi(vec[i].c_str());
	}
	return result;
};
