#include "Algorithm.h"

using namespace std;

Algorithm::Algorithm(void)
{
	dir_="";
	baseName_="";
	extension_="";
	hImages_=0;
	vImages_=0;
	imageRows_=0;
	imageColumns_=0;
	computePosition_=BOTH;
}


Algorithm::~Algorithm(void)
{
}

void Algorithm::compute(string config, double focalPoint) 
{
	if(!EPIExists(config))
		createEPI(config);		
	transform(focalPoint);
}

bool Algorithm::EPIExists(string config)
{
	return EPIExists(config.c_str());
}

bool Algorithm::EPIExists(const char* config) 
{
		//flag holding the result of the method
		bool result=true;
		vector<string> data=fileIO::readFile(config);
		switch(data.at(0).c_str()[0])
		{
		case 'B':
			computePosition_=BOTH;
			break;
		case 'H':
			computePosition_=HORIZONTAL;
			break;
		case 'V':
			computePosition_=VERTICAL;
		};
		dir_=data.at(1);
		baseName_=data.at(2);
		extension_=data.at(3);
		hImages_=atoi(data.at(4).c_str());
		vImages_=atoi(data.at(5).c_str());
		imageRows_=atoi(data.at(6).c_str());
		imageColumns_=atoi(data.at(7).c_str());
		//for (std::vector<int>::iterator it = fifth.begin(); it != fifth.end(); ++it)
		for(int i=0;(i<imageRows_)&&hImages_&&result;i++)
		{
			string path=dir_+baseName_+"_"+utils::to_string<int>(i)+"_1.txt";
			result=fileIO::fileExists(path);
		}
		for(int i=0;(i<imageColumns_)&&vImages_&&result;i++)
		{
			string path=dir_+baseName_+"_"+utils::to_string<int>(i)+"_0.txt";
			result=fileIO::fileExists(path);
		}
		return result;
}

void Algorithm::createEPI(string config)
{
	vector<string> data=fileIO::readFile(config);
	cout<<"Generation started"<<endl;
	//generating files with EPI size
	if(computePosition_==HORIZONTAL||computePosition_==BOTH)
	{
		for(int i=0;i<imageRows_;i++)
		{
			string file=dir_+baseName_+"_"+utils::to_string<int>(i)+"_1.txt";
			string message=utils::to_string<int>(hImages_)+"\n"+utils::to_string<int>(imageColumns_);
			fileIO::saveLine<string>(file, message);
		}
	}
	if(computePosition_==VERTICAL||computePosition_==BOTH)
	{
		for(int i=0;i<imageColumns_;i++)
		{
			string file=dir_+baseName_+"_"+utils::to_string<int>(i)+"_0.txt";
			string message=utils::to_string<int>(imageRows_)+"\n"+utils::to_string<int>(vImages_);
			fileIO::saveLine<string>(file, message);
		}
	}
	//actual creation
	for(int i=0;i<hImages_;i++)
	{
		string source=dir_+"Horizontal\\"+baseName_+utils::to_string<int>(i)+extension_;
		double** tab=graphicIO::getImageInArray(graphicIO::getImage(source), imageRows_, imageColumns_);
		for(int j=0;j<imageRows_;j++)
		{
			string path=dir_+baseName_+"_"+utils::to_string<int>(j)+"_1.txt";
			fileIO::saveLine<double>(path, tab[j], imageColumns_);
		}
		utils::memFree(tab, imageRows_);
	}
	double* temp=new double[imageRows_];
	for(int i=0;i<vImages_;i++)
	{
		string source=dir_+"Vertical\\"+baseName_+utils::to_string<int>(i)+extension_;
		double** tab=graphicIO::getImageInArray(graphicIO::getImage(source), imageRows_, imageColumns_);

		for(int j=0;j<imageColumns_;j++)
		{
			for(int k=0;k<imageRows_;k++)
				temp[k]=tab[k][j];
			string path=dir_+baseName_+"_"+utils::to_string<int>(j)+"_0.txt";
			fileIO::saveLine<double>(path, temp, imageRows_);
		}
		utils::memFree(tab, imageRows_);
	}
	delete[] temp;
	cout<<"Generation ended"<<endl;		
}

int Algorithm::chooseMostReliable()
{
	int imageIndex=0;
	double maxRel=0;
	int rows=0;
	int cols=0;
	if(getComputePosition()==BOTH)
	{
		for(int i=0;i<getHImages();i++)
		{
			double rel=0;
			double** R=fileIO::loadArray(getDir()+getBaseName()+"_finalR_"+to_string(i)+".txt", rows, cols);
			for(int r=0;r<rows;r++)
				for(int c=0;c<cols;c++)
					rel+=R[r][c];
			if(rel>maxRel)
			{
				maxRel=rel;
				imageIndex=i;
			}
			utils::memFree(R, rows);
		}
	}
	else
	{
		if(getComputePosition()==HORIZONTAL)
		{
			for(int i=0;i<getHImages();i++)
			{
				double rel=0;
				double** R=fileIO::loadArray(getDir()+getBaseName()+to_string(i)+"_1.txt", rows, cols);
				for(int r=0;r<rows;r++)
					for(int c=0;c<cols;c++)
						rel+=R[r][c];
				if(rel>maxRel)
				{
					maxRel=rel;
					imageIndex=i;
				}
				utils::memFree(R, rows);
			}
		}
		else
		{
			for(int i=0;i<getVImages();i++)
			{
				double rel=0;
				double** R=fileIO::loadArray(getDir()+getBaseName()+"_r_"+to_string(i)+"_0.txt", rows, cols);
				for(int r=0;r<rows;r++)
					for(int c=0;c<cols;c++)
						rel+=R[r][c];
				if(rel>maxRel)
				{
					maxRel=rel;
					imageIndex=i;
				}
				utils::memFree(R, rows);
			}
		}
	}
	return imageIndex;
}