#include "Disney.h"


Disney::Disney(void)
{
}


Disney::~Disney(void)
{
}

double Disney::getCe(double* row, int length, int index, int size)
{
	double result=0;
	int start=index-size/2;
	int end=index+size/2;
	if(start<0)
	{
		end+=0-start;
		start=0;
	}
	if(end>length-1)
	{
		start-=end-length+1;
		end=length-1;
	}
	for(;start<=end;start++)
		result+=(row[start]-row[index])*(row[start]-row[index]);
	return result;
}

void Disney::transform(double focal)
{
	for(int i=0;i<getImageRows();i++)
	{
		EPI epi(EPI::createEPIPath(getDir(), 1, getBaseName(), i, HORIZONTAL));
		cout<<"Procesing "<<i+1<<" EPI from "<<getImageRows()<<endl;
		//from [0-255] range to [0-1]
		double** tempPixels=graphicUtils::toGrayScale(epi.getPixels(), epi.getRows(), epi.getColumns(), GrayMethod::LIGHTNESS);
		double** pixels=graphicUtils::pixelMultiply(1.0/255.0, tempPixels, epi.getRows(), epi.getColumns());

		utils::memFree(tempPixels, epi.getRows());

		//equation(2)
		double** Ce=utils::create2DDouble(epi.getRows(), epi.getColumns());
		int** Me=utils::create2DInt(epi.getRows(), epi.getColumns());
		for(int i=0;i<epi.getRows();i++)
			for(int j=0;j<epi.getColumns();j++)
			{
				Ce[i][j]=getCe(pixels[i], epi.getColumns(), j, NEIGHBORS);
				if(Ce[i][j]<THRESHOLD)
					Me[i][j]=0;
				else Me[i][j]=1;
			}
		int** temp=graphicUtils::opening(Me, epi.getRows(), epi.getColumns(), 3, 3);
		utils::memFree(Me, epi.getRows());
		Me=temp;

		utils::memFree(Me, epi.getRows());
		utils::memFree(pixels, epi.getRows());
		utils::memFree(Ce, epi.getRows());
	}
}

void Disney::makeConsistent()
{
}
