#include "Selector.h"

using namespace std;

vector<double> selector::select(string file, string source, double minZ, double maxZ, double minX, double maxX, double minY, double maxY)
{
	return select(file.c_str(), file.c_str(), minZ, maxZ, minX, maxX, minY, maxY);
}

vector<double> selector::select(char* file, char* source, double minZ, double maxZ, double minX, double maxX, double minY, double maxY)
{
	cout<<"Selecting point from image"<<endl;
	vector<double> result;
	int rows;
	int cols;
	double** image=graphicIO::getImageInArray(source, rows, cols);
	double** gray=graphicUtils::toGrayScale(image, rows, cols, AVERAGE);
	double** tab=fileIO::loadArray(file, rows, cols);
	int last=0;//when was last one choosen
	for(int y=minX;y<cols;y++)
		for(int x=minY;x<rows;x++)
		{
			if(gray[x][y]<180&&tab[x][y]>minZ&&tab[x][y]<maxZ&&last>LAST)
			{
				result.push_back(y);//moving to image coordinates
				result.push_back(x);//moving to image coordinates
				result.push_back(tab[x][y]);
				last=0;
			}
			else
				last++;
		}
	utils::memFree(tab, rows);
	utils::memFree(image, rows);
	utils::memFree(gray, rows);
	cout<<"Selection done"<<endl;
	return result;
}