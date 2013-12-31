#include <iostream>
#include <cstdlib>
#include <vector>
#include "Algorithm.h"
#include "CDL.h"
#include "Disney.h"
#include "Position.h"
#include "Selector.h"
#include "Transfomation.h"
#include "Triangulation.h"
#include "FileIO.h"

using namespace std;

void run(int algorithm, vector<string> mainConfig, double focal, string outputFile)
{
	Algorithm *a;
	if(algorithm==0)
	{
		CDL cdl;
		a=&cdl;
	}
	else
	{
		Disney disney;
		a=&disney;
	}
	for(int i=0;i<mainConfig.size();i++)
		a->compute(mainConfig[i], focal);
	int imageNr=a->chooseMostReliable();

	vector<double> vec;
	for(int i=0;i<mainConfig.size();i++)
	{
		vector<double> temp=selector::select(mainConfig[i], i);
		transformation::transform(mainConfig[i],temp);
		vec.insert(vec.end(), temp.begin(), temp.end());
	}
	double* points = &vec[0];
	triangulation::triangulate(vec.size(), points, outputFile);		
}

int main(int argc, const char* argv[])
{
	CDL cdl;
	if(argc>1)
	{
		cdl.compute(argv[1], std::atof(argv[3]));
	}
	else
		cdl.compute("D:\\bikes\\config.txt", 7800);
	system("pause");
}

