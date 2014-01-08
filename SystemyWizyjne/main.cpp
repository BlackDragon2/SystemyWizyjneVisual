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
#include "GraphicIO.h"

using namespace std;

void run(int algorithm, vector<string> mainConfig, double focal, char* outputFile, double min, double max, double shift)
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
		vector<string> config=fileIO::readFile(mainConfig[i]);
		string file=config[1]+config[2]+"_";
		switch(config.at(0).c_str()[0])
		{
		case 'B':
			file+"final_"+to_string(imageNr)+".txt";
			break;
		case 'H':
			file+"image_"+to_string(imageNr)+"_1.txt";
			break;
		case 'V':
			file+"image_"+to_string(imageNr)+"_0.txt";
		};
		vector<double> temp=selector::select(file, min, max, shift);
		string slope=utils::split(config[2],"_")[1];
		transformation::transform(temp, atof(slope.c_str()));
		vec.insert(vec.end(), temp.begin(), temp.end());
	}
	double* points = &vec[0];
	triangulation::triangulate(vec.size(), points, outputFile);		
}

void run(int algorithm, vector<string> mainConfig, double focal, string outputFile)
{
	run(algorithm, mainConfig, focal, outputFile.c_str());
}

int main(int argc, const char* argv[])
{
	//int k=5785;
	//string old="D:\\car\\IMG_";
	//string newOne="D:\\car\\car";
	//for(int i=0;i<289;i++)
	//{
	//	string o=old+to_string(k+i)+".JPG";
	//	string n=newOne+to_string(i)+".jpg";
	//	rename(o.c_str(), n.c_str());
	//}
	//for(int i=0;i<289;i++)
	//{
	//	string n=newOne+to_string(i)+".jpg";
	//	graphicIO::saveImage(graphicIO::scaleImage(graphicIO::getImage(n), 648, 972), n);
	//}
	CDL cdl;
	if(argc>1)
	{
		cdl.compute(argv[1], std::atof(argv[3]));
	}
	else
	{
		cdl.compute("D:\\car\\config.txt", 7800);
		cdl.visualize("D:\\car\\car_final_145.txt", "D:\\other\\car.jpg", 500, 20000);
		//cdl.compute("D:\\car2\\config.txt", 7800);
		//cdl.visualize("D:\\car2\\car_image_4_0.txt", "D:\\other\\car2.jpg", 500, 20000);
		//cdl.compute("D:\\bikes\\config.txt", 7800);
		//cdl.visualize("D:\\bikes\\bikes_image_25_1.txt", "D:\\other\\bikesWithoutGaussian.jpg", 1000, 15000);
	}
	system("pause");
}

//int main(int argc, const char* argv[])
//{
//	if(argc>1)
//	{
//		run(atoi(argv[1]), fileIO::readFile(argv[2]), atof(argv[3]), argv[4]);
//	}
//	else
//		run(0, fileIO::readFile(""), 7800, "D:\\other\\result.poly", 1000, 30000, 100);
//	system("pause");
//}

