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

void run(int algorithm, vector<string> mainConfig, double focal, char* outputFile, double minZ, double maxZ)
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
	//int imageNr=a->chooseMostReliable();
	int imageNr=2;
	vector<double> vec;
	for(int i=0;i<mainConfig.size();i++)
	{
		vector<string> config=fileIO::readFile(mainConfig[i]);
		string file=config[1]+config[2]+"_";
		string source=config[1];
		switch(config.at(0).c_str()[0])
		{
		case 'B':
			file=file+"final_"+to_string(imageNr)+".txt";
			source=source+"horizontal\\"+config[2]+"_"+to_string(imageNr)+config[3];
			break;
		case 'H':
			file=file+"image_"+to_string(imageNr)+"_1.txt";
			source=source+"horizontal\\"+config[2]+"_"+to_string(imageNr)+config[3];
			break;
		case 'V':
			file=file+"image_"+to_string(imageNr)+"_0.txt";
			source=source+"vertical\\"+config[2]+"_"+to_string(imageNr)+config[3];
		};
		vector<double> temp=selector::select(file, source, minZ, maxZ, 170, 580, 120, 320);//in image coordinates
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


//int main(int argc, const char* argv[])
//{
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
	//CDL cdl;
	//if(argc>1)
	//{
	//	cdl.compute(argv[1], std::atof(argv[3]));
	//}
	//else
	//{
		//cdl.compute("D:\\car\\config.txt", 7800);
		//cdl.visualize("D:\\car\\car_final_145.txt", "D:\\other\\carX.jpg", 500, 20000);
		//cdl.compute("D:\\car2\\config.txt", 7800);
		//cdl.visualize("D:\\car2\\car_image_4_0.txt", "D:\\other\\car2.jpg", 500, 20000);
		//cdl.compute("D:\\bikes\\config.txt", 7800);
		//cdl.visualize("D:\\bikes\\bikes_image_25_1.txt", "D:\\other\\bikesLastStand2.jpg", 1000, 15000);
		//cdl.compute("D:\\pies\\config.txt", 7800);
		//cdl.visualize("D:\\pies\\mysz_image_2_1.txt", "D:\\other\\MyszStandardNoBlurStructManyBlurs.jpg", 100, 15000);
		//cdl.compute("D:\\zapalniczka\\config.txt", 7800);
		//cdl.visualize("D:\\zapalniczka\\zapalniczka_image_2_1.txt", "D:\\other\\zapalniczka.jpg", 100, 15000);
		//cdl.compute("D:\\bikes2\\config.txt", 7800);
		//cdl.visualize("D:\\bikes2\\bikes_image_12_1.txt", "D:\\other\\gradientOnly.jpg", 1000, 15000);
		//cdl.compute("D:\\bikes3\\config.txt", 7800);
		//cdl.visualize("D:\\bikes3\\bikes_image_2_1.txt", "D:\\other\\chybaFinal.jpg", 0, 6000);
	//}
	//system("pause");
//}

int main(int argc, const char* argv[])
{
	if(argc>1)
	{
		run(atoi(argv[1]), fileIO::readFile(argv[2]), atof(argv[3]), argv[4]);
	}
	else
		run(0, fileIO::readFile("D:\\mysz\\configMain.txt"), 1000, "D:\\other\\result.poly", 100, 3000);
	system("pause");
}

