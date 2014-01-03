#include "Selector.h"

using namespace std;

vector<double> selector::select(std::string file, double min, double max, double dist)
{
	return select(file.c_str(), min, max, dist);
}

vector<double> selector::select(char* file, double min, double max, double dist)
{
	vector<double> result;
	int rows;
	int cols;
	double** tab=fileIO::loadArray(file, rows, cols);
	//double lastX=0;
	//double lastY=0;
	double lastZ=0;
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
		{
			if(tab[i][j]>min&&tab[i][j]<max&&(tab[i][j]-lastZ)*(tab[i][j]-lastZ)>dist)
			{
				result.push_back(i);
				result.push_back(j);
				result.push_back(tab[i][j]);
				lastZ=tab[i][j];
			}
		}
	utils::memFree(tab, rows);
	return result;
}