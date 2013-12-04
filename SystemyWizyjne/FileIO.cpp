#include "FileIO.h"
#include "Utils.h"

using namespace std;

bool fileIO::fileExists(string file)
{
    ifstream infile(file.c_str());
    return infile.good();
};

bool fileIO::fileExists(const char* file)
{
    ifstream infile(file);
    return infile.good();
};

void fileIO::saveArray(string file, double** matrix, int rows, int columns, bool reverse)
{
	saveArray(file.c_str(), matrix, rows, columns, reverse);
};

void fileIO::saveArray(string file, int** matrix, int rows, int columns, bool reverse)
{
	saveArray(file.c_str(), matrix, rows, columns, reverse);
};

void fileIO::saveArray(const char* file, double** matrix, int rows, int columns, bool reverse)
{
	if(!fileIO::fileExists(file))
	{
		ofstream saveFile;
		saveFile.open(file);
		saveFile << rows << endl;
		saveFile << columns << endl;
		if(!reverse)
		{
			for(int i=0;i<rows;i++)
				for(int j=0;j<columns;j++)
				{
					saveFile << matrix[i][j];
					if(j==columns-1)
						saveFile << endl;
					else 
						saveFile << " ";
				}
		}
		else
		{
			for(int j=0;j<columns;j++)
				for(int i=0;i<rows;i++)
				{
					saveFile << matrix[i][j];
					if(i==rows-1)
						saveFile << endl;
					else 
						saveFile << " ";
				}
		}
		saveFile.close();
	}
};

void fileIO::saveArray(const char* file, int** matrix, int rows, int columns, bool reverse)
{
	if(!fileIO::fileExists(file))
	{
		ofstream saveFile;
		saveFile.open(file);
		saveFile << rows << endl;
		saveFile << columns << endl;
		if(!reverse)
		{
			for(int i=0;i<rows;i++)
				for(int j=0;j<columns;j++)
				{
					saveFile << matrix[i][j];
					if(j==columns-1)
						saveFile << endl;
					else 
						saveFile << " ";
				}
		}
		else
		{
			for(int j=0;j<columns;j++)
				for(int i=0;i<rows;i++)
				{
					saveFile << matrix[i][j];
					if(i==rows-1)
						saveFile << endl;
					else 
						saveFile << " ";
				}
		}
		saveFile.close();
	}
};

double** fileIO::loadArray(string file, int &rows, int &columns)
{
	return loadArray(file.c_str(), rows, columns);
};

double** fileIO::loadArray(const char* file, int &rows, int &columns)
{
	ifstream sourceFile(file);
	double** result=0;
	string line;
	if (sourceFile.is_open())
	{
		getline(sourceFile,line);
		rows=atoi(line.c_str());
		getline(sourceFile,line);
		columns=atoi(line.c_str());
		result=new double*[rows];
		double** pointer=result;
		while ( getline (sourceFile,line) )
		{
			(*pointer)=utils::parseDouble(utils::split(line, " "));
			pointer++;
		}
		sourceFile.close();
	}
	else 
		cout << "Unable to open file";
	return result;
};

vector<string> fileIO::readFile(string file)
{
	return readFile(file.c_str());
};

vector<string> fileIO::readFile(const char* file)
{
	ifstream sourceFile(file);
	string line;
	vector<string> lines;
	while ( getline (sourceFile,line) )
	{
		lines.push_back(line);
	}
	return lines;
};

