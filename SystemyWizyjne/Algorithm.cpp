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

void Algorithm::visualize(const char* outFile, double** arr, int rows, int cols, double minimum, double maximum)
{
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			if(arr[i][j]>maximum)
				arr[i][j]=maximum;
			if(arr[i][j]<minimum)
				arr[i][j]=minimum;
			arr[i][j]=255-((arr[i][j]-minimum)/(maximum-minimum))*255;//255-... so that black(0,0,0) corresponds to greatest distance
		}
	}
	graphicIO::saveImage(arr, rows, cols, outFile, 1);
}

void Algorithm::visualize(string outFile, double** arr, int rows, int cols, double minimum, double maximum)
{
	visualize(outFile.c_str(), arr, rows, cols, minimum, maximum);
}

void Algorithm::visualize(char* outFile, double** arr, int rows, int cols)
{
	visualize(outFile, arr, rows, cols, 0.0, 1.0);
}

void Algorithm::visualize(string outFile, double** arr, int rows, int cols)
{
	visualize(outFile.c_str(), arr, rows, cols, 0.0, 1.0);
}

void Algorithm::visualize(char* inFile, char* outFile, double minimum, double maximum)
{
	int rows;
	int cols;
	double** tab=fileIO::loadArray(inFile, rows, cols);
	visualize(outFile, tab, rows, cols, minimum, maximum);
}

void Algorithm::visualize(char* inFile, char* outFile)
{
	visualize(inFile, outFile, 0.0, 1.0);
}

void Algorithm::visualize(string inFile, string outFile, double minimum, double maximum)
{
	visualize(inFile.c_str(), outFile.c_str(), minimum, maximum);
}

void Algorithm::visualize(string inFile, string outFile)
{
	visualize(inFile.c_str(), outFile.c_str(), 0.0, 1.0);
}

void Algorithm::compute(string config, double focalPoint) 
{
	if(!EPIExists(config))
		createEPI();		
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
		
		int groupID=0;
		string group="";
		if(computePosition_==BOTH)
			groupID=vImages_;
		else groupID=1;
		while(groupID)
		{
			group=to_string(groupID);
			for(int i=0;(i<imageRows_)&&hImages_&&result;i++)
			{
				string path=dir_+group+baseName_+"_"+to_string(i)+"_1.txt";
				result=fileIO::fileExists(path);
			}
			groupID--;
		}
		if(computePosition_==BOTH)
			groupID=hImages_;
		else groupID=1;
		while(groupID)
		{
			group=to_string(groupID);
			for(int i=0;(i<imageColumns_)&&vImages_&&result;i++)
			{
				string path=dir_+group+baseName_+"_"+to_string(i)+"_0.txt";
				result=fileIO::fileExists(path);
			}
			groupID--;
		}
		return result;
}

void Algorithm::createEPI()
{
	cout<<"Generation started"<<endl;
	//generating files with EPI size
	if(computePosition_==BOTH)
	{
		for(int i=1;i<=vImages_;i++)
			createHorizontalEPI(i);
		for(int i=1;i<=hImages_;i++)
			createVerticalEPI(i, 1);
	}
	if(computePosition_==HORIZONTAL)
	{
		createHorizontalEPI(1);
	}
	if(computePosition_==VERTICAL)
	{
		createVerticalEPI(1, 0);
	}
	cout<<"Generation ended"<<endl;		
}

void Algorithm::createHorizontalEPI(int groupID)
{
	for(int i=0;i<imageRows_;i++)
	{
		string file=dir_+to_string(groupID)+baseName_+"_"+to_string(i)+"_1.txt";
		string message=to_string(hImages_)+"\n"+to_string(imageColumns_);
		fileIO::saveLine<string>(file, message);
	}
	for(int i=0;i<hImages_;i++)
	{
		string source=dir_+"Horizontal\\"+baseName_+to_string(i+(groupID-1)*hImages_)+extension_;
		double** tab=graphicIO::getImageInArray(graphicIO::getImage(source), imageRows_, imageColumns_);
		for(int j=0;j<imageRows_;j++)
		{
			string path=dir_+to_string(groupID)+baseName_+"_"+to_string(j)+"_1.txt";
			fileIO::saveLine<double>(path, tab[j], imageColumns_);
		}
		utils::memFree(tab, imageRows_);
	}
}

void Algorithm::createVerticalEPI(int groupID, char mul)
{
	for(int i=0;i<imageColumns_;i++)
	{
		string file=dir_+to_string(groupID)+baseName_+"_"+to_string(i)+"_0.txt";
		string message=to_string(imageRows_)+"\n"+to_string(vImages_);
		fileIO::saveLine<string>(file, message);
	}
	double* temp=new double[imageRows_];
	for(int i=0;i<vImages_;i++)
	{
		string source=dir_+"Vertical\\"+baseName_+to_string(i*hImages_+i*mul)+extension_;
		double** tab=graphicIO::getImageInArray(graphicIO::getImage(source), imageRows_, imageColumns_);

		for(int j=0;j<imageColumns_;j++)
		{
			for(int k=0;k<imageRows_;k++)
				temp[k]=tab[k][j];
			string path=dir_+to_string(groupID)+baseName_+"_"+to_string(j)+"_0.txt";
			fileIO::saveLine<double>(path, temp, imageRows_);
		}
		utils::memFree(tab, imageRows_);
	}
	delete[] temp;
}

int Algorithm::chooseMostReliable()
{
	int imageIndex=0;
	double maxRel=0;
	int rows=0;
	int cols=0;
	if(getComputePosition()==BOTH)
	{
		for(int i=0;i<getHImages()*getVImages();i++)
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

void Algorithm::changeName(string type)
{
	for(int i=0;i<getVImages()+getHImages();i++)
	{
		string oldFile=getDir()+getBaseName()+"_image_"+to_string(i)+type;
		string newFile=getDir()+getBaseName()+"_final_"+to_string(i)+".txt";
		rename( oldFile.c_str() , newFile.c_str());
		oldFile=getDir()+getBaseName()+"_r_"+to_string(i)+type;
		newFile=getDir()+getBaseName()+"_finalR_"+to_string(i)+".txt";
	}
}