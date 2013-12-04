#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace fileIO
{
	template<typename T>
	void saveLine(std::string path, T param)
	{
		saveLine(path.c_str(), param);
	};
	template<typename T>
	void save(const char* path, T param)
	{
		std::ofstream outFile;
		outFile.open(path, std::ios::out | std::ios::app );
		outFile<<param;
		outFile.close();
	};
	template<typename T>
	void save(std::string path, T param)
	{
		save(path.c_str(), param);
	};
	template<typename T>
	void saveLine(const char* path, T param)
	{
		std::ofstream outFile;
		outFile.open(path, std::ios::out | std::ios::app);
		outFile<<param;
		outFile<<std::endl;
		outFile.close();
	};
	template<typename T>
	void saveLine(const char* path, T* param, int elemCount)
	{
		std::ofstream outFile;
		outFile.open(path, std::ios::out | std::ios::app );
		for(int i=0;i<elemCount;i++)
		{
			outFile<<param[i];
			if(!(i-1==elemCount))
				outFile<<" ";
		}
		outFile<<std::endl;
		outFile.close();
	};

	template<typename T>
	void saveLine(std::string path, T* param, int elemCount)
	{
		saveLine(path.c_str(), param, elemCount);
	};

	bool fileExists(std::string file);
	bool fileExists(const char* file);
	void saveArray(std::string file, double** matrix, int rows, int columns, bool reverse);
	void saveArray(std::string file, int** matrix, int rows, int columns, bool reverse);
	void saveArray(const char* file, double** matrix, int rows, int columns, bool reverse);
	void saveArray(const char* file, int** matrix, int rows, int columns, bool reverse);
	double** loadArray(std::string file, int &rows, int &columns);
	double** loadArray(const char* file, int &rows, int &columns);
	std::vector<std::string> readFile(std::string file);
	std::vector<std::string> readFile(const char* file);
};


