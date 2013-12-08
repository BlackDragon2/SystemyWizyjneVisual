#include "CDL.h"


CDL::CDL(void) : Algorithm()
{
}


CDL::~CDL(void)
{
}

void CDL::transform(double focal)
{
	double** Jxx;
	double** Jxy;
	double** Jyy;
	double** HImage;
	double** VImage;
	double* Hr=new double[getHImages()];
	double* Vr=new double[getVImages()];
	double** r;
	if(getComputePosition()==HORIZONTAL)
	{
		createFiles();
		for(int i=0;i<getImageRows();i++)
		{
			EPI epi(EPI::createEPIPath(getDir(), getBaseName(), i, HORIZONTAL));
			//from [0-255] range to [0-1]
			double** pixels=graphicUtils::pixelMultiply(double(1/255), epi.getPixels(), epi.getRows(), epi.getColumns()); 

			double** gradientX=graphicUtils::gradient(MASKX, pixels, epi.getRows(), epi.getColumns());
			double** gradientY=graphicUtils::gradient(MASKY, pixels, epi.getRows(), epi.getColumns());
			double** gaussian=graphicUtils::normalize(graphicUtils::GaussianKernel(PHI, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS), GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, true);
			Jxx=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientX, gradientX, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
			Jxy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientX, gradientY, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
			Jyy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientY, gradientY, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
				
			//equation(5)
			double** sub=graphicUtils::pixelSubstract(Jyy, Jxx, epi.getRows(), epi.getColumns());
			double** mul=graphicUtils::pixelMultiply(2.0, Jxy, epi.getRows(), epi.getColumns());
			
			double** dev=graphicUtils::pixelDivide(sub, mul, epi.getRows(), epi.getColumns());
			double** arctan=mathUtils::arctan(dev, epi.getRows(), epi.getColumns());
			double** phi=graphicUtils::pixelMultiply(0.5, arctan, epi.getRows(), epi.getColumns());
			
			//equation(7)

			double** sum=graphicUtils::pixelAdd(Jxx, Jyy, epi.getRows(), epi.getColumns());
			double** sumQuater=graphicUtils::pixelMultiply(sum, sum, epi.getRows(), epi.getColumns());

			double** subQuater=graphicUtils::pixelMultiply(sub, sub, epi.getRows(), epi.getColumns());
			double** mulQuater=graphicUtils::pixelMultiply(mul, mul, epi.getRows(), epi.getColumns());
			double** subPlusMul=graphicUtils::pixelAdd(subQuater, mulQuater, epi.getRows(), epi.getColumns());

			r=graphicUtils::pixelDivide(subPlusMul, sumQuater, epi.getRows(), epi.getColumns());
			double* message=new double[epi.getColumns()];
			for(int i=0;i<epi.getRows();i++)
			{
				
				for(int j=0;j<epi.getColumns();j++)
					message[j]=-focal*(1/tan(phi[i][j]))*255;
				string file=getDir()+getBaseName()+"_image_"+utils::to_string<int>(i)+"_1.txt";
				fileIO::saveLine(file, message, epi.getColumns());
				file=getDir()+getBaseName()+"_r_"+utils::to_string<int>(i)+"_1.txt";
				fileIO::saveLine(file, r[i], epi.getColumns());
			}
			
			utils::memFree(dev, epi.getRows());
			utils::memFree(sub, epi.getRows());
			utils::memFree(mul, epi.getRows());
			utils::memFree(sum, epi.getRows());
			utils::memFree(subQuater, epi.getRows());
			utils::memFree(mulQuater, epi.getRows());
			utils::memFree(sumQuater, epi.getRows());
			utils::memFree(subPlusMul, epi.getRows());
			utils::memFree(arctan, epi.getRows());
			utils::memFree(pixels, epi.getRows());
			utils::memFree(gradientX, epi.getRows());
			utils::memFree(gradientY, epi.getRows());
			utils::memFree(gaussian, epi.getRows());
			utils::memFree(Jxx, epi.getRows());
			utils::memFree(Jxy, epi.getRows());
			utils::memFree(Jyy, epi.getRows());
			//dX=mathUtils::sinus(phi, epi.getRows(), epi.getColumns()); 
			//dS=mathUtils::cosinus(phi, epi.getRows(), epi.getColumns());
				//equation(6)
			
		}
	}
	if(getComputePosition()==VERTICAL)
	{
		for(int i=0;i<getImageColumns();i++)
		{
			EPI epi(EPI::createEPIPath(getDir(), getBaseName(), i, VERTICAL));

			//from [0-255] range to [0-1]
			double** pixels=graphicUtils::pixelMultiply(double(1/255), epi.getPixels(), epi.getRows(), epi.getColumns()); 

			double** gradientX=graphicUtils::gradient(MASKX, pixels, epi.getRows(), epi.getColumns());
			double** gradientY=graphicUtils::gradient(MASKY, pixels, epi.getRows(), epi.getColumns());
			double** gaussian=graphicUtils::normalize(graphicUtils::GaussianKernel(PHI, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS), GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, true);
			Jxx=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientX, gradientX, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
			Jxy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientX, gradientY, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
			Jyy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientY, gradientY, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
				
			//equation(5)
			double** sub=graphicUtils::pixelSubstract(Jyy, Jxx, epi.getRows(), epi.getColumns());
			double** mul=graphicUtils::pixelMultiply(2.0, Jxy, epi.getRows(), epi.getColumns());
			
			double** dev=graphicUtils::pixelDivide(sub, mul, epi.getRows(), epi.getColumns());
			double** arctan=mathUtils::arctan(dev, epi.getRows(), epi.getColumns());
			double** phi=graphicUtils::pixelMultiply(0.5, arctan, epi.getRows(), epi.getColumns());
			
			//equation(7)

			double** sum=graphicUtils::pixelAdd(Jxx, Jyy, epi.getRows(), epi.getColumns());
			double** sumQuater=graphicUtils::pixelMultiply(sum, sum, epi.getRows(), epi.getColumns());

			double** subQuater=graphicUtils::pixelMultiply(sub, sub, epi.getRows(), epi.getColumns());
			double** mulQuater=graphicUtils::pixelMultiply(mul, mul, epi.getRows(), epi.getColumns());
			double** subPlusMul=graphicUtils::pixelAdd(subQuater, mulQuater, epi.getRows(), epi.getColumns());

			r=graphicUtils::pixelDivide(subPlusMul, sumQuater, epi.getRows(), epi.getColumns());
			double* message=new double[epi.getRows()];
			double* rMessage=new double[epi.getRows()];
			for(int i=0;i<epi.getColumns();i++)
			{
				
				for(int j=0;j<epi.getRows();j++)
				{
					message[j]=-focal*(1/tan(phi[j][i]))*255;
					rMessage[j]=r[j][i];
				}
				string file=getDir()+getBaseName()+"_image_"+utils::to_string<int>(i)+"_0.txt";
				fileIO::saveLine(file, message, epi.getRows());
				file=getDir()+getBaseName()+"_r_"+utils::to_string<int>(i)+"_0.txt";
				fileIO::saveLine(file, rMessage, epi.getRows());
			}
			
			utils::memFree(dev, epi.getRows());
			utils::memFree(sub, epi.getRows());
			utils::memFree(mul, epi.getRows());
			utils::memFree(sum, epi.getRows());
			utils::memFree(subQuater, epi.getRows());
			utils::memFree(mulQuater, epi.getRows());
			utils::memFree(sumQuater, epi.getRows());
			utils::memFree(subPlusMul, epi.getRows());
			utils::memFree(arctan, epi.getRows());
			utils::memFree(pixels, epi.getRows());
			utils::memFree(gradientX, epi.getRows());
			utils::memFree(gradientY, epi.getRows());
			utils::memFree(gaussian, epi.getRows());
			utils::memFree(Jxx, epi.getRows());
			utils::memFree(Jxy, epi.getRows());
			utils::memFree(Jyy, epi.getRows());
		}
	}
}

void CDL::createFiles()
{
	if(getComputePosition()==Position::HORIZONTAL)
	{
		for(int i=0;i<getHImages();i++)
		{
			string file=getDir()+getBaseName()+"_image_"+utils::to_string<int>(i)+"_1.txt";
			string message=utils::to_string<int>(getImageRows())+"\n"+utils::to_string<int>(getImageColumns());
			fileIO::saveLine<string>(file, message);
			file=getDir()+getBaseName()+"_r_"+utils::to_string<int>(i)+"_1.txt";
			message=utils::to_string<int>(getImageRows())+"\n"+utils::to_string<int>(getImageColumns());
			fileIO::saveLine<string>(file, message);
		}
	}
	else
	{
		for(int i=0;i<getVImages();i++)
		{
			string file=getDir()+getBaseName()+"_image_"+utils::to_string<int>(i)+"_0.txt";
			string message=utils::to_string<int>(getImageRows())+"\n"+utils::to_string<int>(getImageColumns());
			fileIO::saveLine<string>(file, message);
			file=getDir()+getBaseName()+"_r_"+utils::to_string<int>(i)+"_0.txt";
			message=utils::to_string<int>(getImageRows())+"\n"+utils::to_string<int>(getImageColumns());
			fileIO::saveLine<string>(file, message);
		}
	}
}
