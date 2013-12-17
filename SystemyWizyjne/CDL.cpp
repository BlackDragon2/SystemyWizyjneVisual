#include "CDL.h"

CDL::CDL(void) : Algorithm()
{
}


CDL::~CDL(void)
{
}

void visualize(double** arr=0, int rows=0, int cols=0)
{
	if(arr==0)
		arr=fileIO::loadArray("D:\\bikes\\bikes_image_26_1.txt", rows, cols);
	/*for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
			if(arr[i][j]<0)
				arr[i][j]=0;*/
	double min=arr[0][0];
	double max=arr[0][0];
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
		{
			if(arr[i][j]<min)
				min=arr[i][j];
			if(arr[i][j]>max)
				max=arr[i][j];
		}
	if(max>10000)
		max=10000;
	if(min<0)
		min=0;
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
		{
			arr[i][j]=((arr[i][j]-min)/(max-min))*255;
			if(arr[i][j]>255) arr[i][j]=255;
			if(arr[i][j]<0) arr[i][j]=0;
		}
	graphicIO::showImage(arr, rows, cols, 1);
	graphicIO::saveImage(arr, rows, cols, "D:\\other\\im1.jpg", 1);
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++)
		{
			arr[i][j]=255-arr[i][j];
		}
	cout<<"Min "<<min<<endl;
	cout<<"Max "<<max<<endl;
	graphicIO::saveImage(arr, rows, cols, "D:\\other\\im2.jpg", 1);
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

		//EPI epi(EPI::createEPIPath(getDir(), getBaseName(), 0, HORIZONTAL));
		//graphicIO::showImage(epi.getPixels(), epi.getRows(), epi.getColumns(), 1);
		

		double** gaussian=graphicUtils::normalize(graphicUtils::GaussianKernel(PHI, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS), GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, true);
		for(int i=0;i<getImageRows();i++)
		{
			cout<<"Procesing "<<i+1<<" EPI from "<<getImageRows()<<endl;
			EPI epi(EPI::createEPIPath(getDir(), getBaseName(), i, HORIZONTAL));
			//from [0-255] range to [0-1]
			double** pixels=graphicUtils::pixelMultiply(1.0/255.0, epi.getPixels(), epi.getRows(), epi.getColumns()); 
	
			double** gradientX=graphicUtils::gradient(MASKX, pixels, epi.getRows(), epi.getColumns());
			double** gradientY=graphicUtils::gradient(MASKY, pixels, epi.getRows(), epi.getColumns());
			double** xx=graphicUtils::pixelMultiply(gradientX, gradientX, epi.getRows(), epi.getColumns());
			double** xy=graphicUtils::pixelMultiply(gradientX, gradientY, epi.getRows(), epi.getColumns());
			double** yy=graphicUtils::pixelMultiply(gradientY, gradientY, epi.getRows(), epi.getColumns());
			
			//graphicIO::showImage(graphicUtils::pixelMultiply(255, gradientX, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns(), 1);
			Jxx=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, xx, epi.getRows(), epi.getColumns());//graphicIO::showImage(graphicUtils::pixelMultiply(255, Jxx, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns(), 1);
			Jxy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, xy, epi.getRows(), epi.getColumns());
			Jyy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, yy, epi.getRows(), epi.getColumns());

			//equation(5)
			double** sub=graphicUtils::pixelSubstract(Jyy, Jxx, epi.getRows(), epi.getColumns());
			double** mul=graphicUtils::pixelMultiply(2.0, Jxy, epi.getRows(), epi.getColumns());
			
			double** arctan=mathUtils::arctan(mul, sub, epi.getRows(), epi.getColumns());
			double** phi=graphicUtils::pixelMultiply(0.5, arctan, epi.getRows(), epi.getColumns());
	
			//equation(7)
			//visualize(phi, epi.getRows(), epi.getColumns());
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
					message[j]=focal/(tan(phi[i][j])+1);//message[j]=-focal*(tan(phi[i][j]))*255;
				string file=getDir()+getBaseName()+"_image_"+utils::to_string<int>(i)+"_1.txt";
				fileIO::saveLine(file, message, epi.getColumns());
				file=getDir()+getBaseName()+"_r_"+utils::to_string<int>(i)+"_1.txt";
				fileIO::saveLine(file, r[i], epi.getColumns());
			}
			utils::memFree(xx, epi.getRows());
			utils::memFree(xy, epi.getRows());
			utils::memFree(yy, epi.getRows());
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
			utils::memFree(Jxx, epi.getRows());
			utils::memFree(Jxy, epi.getRows());
			utils::memFree(Jyy, epi.getRows());
			//dX=mathUtils::sinus(phi, epi.getRows(), epi.getColumns()); 
			//dS=mathUtils::cosinus(phi, epi.getRows(), epi.getColumns());
				//equation(6)
			
		}
		utils::memFree(gaussian, 3);
	}
	if(getComputePosition()==VERTICAL)
	{
		for(int i=0;i<getImageColumns();i++)
		{
			EPI epi(EPI::createEPIPath(getDir(), getBaseName(), i, VERTICAL));

			//from [0-255] range to [0-1]
			double** pixels=graphicUtils::pixelMultiply(1.0/255.0, epi.getPixels(), epi.getRows(), epi.getColumns()); 

			double** gradientX=graphicUtils::gradient(MASKX, pixels, epi.getRows(), epi.getColumns());
			double** gradientY=graphicUtils::gradient(MASKY, pixels, epi.getRows(), epi.getColumns());
			double** gaussian=graphicUtils::normalize(graphicUtils::GaussianKernel(PHI, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS), GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, true);
			Jxx=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientX, gradientX, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
			Jxy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientX, gradientY, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
			Jyy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientY, gradientY, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
				
			//equation(5)
			double** sub=graphicUtils::pixelSubstract(Jyy, Jxx, epi.getRows(), epi.getColumns());
			double** mul=graphicUtils::pixelMultiply(2.0, Jxy, epi.getRows(), epi.getColumns());
			
			double** arctan=mathUtils::arctan(mul, sub, epi.getRows(), epi.getColumns());
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
					message[j]=focal/(tan(phi[j][i])+1);
					rMessage[j]=r[j][i];
				}
				string file=getDir()+getBaseName()+"_image_"+utils::to_string<int>(i)+"_0.txt";
				fileIO::saveLine(file, message, epi.getRows());
				file=getDir()+getBaseName()+"_r_"+utils::to_string<int>(i)+"_0.txt";
				fileIO::saveLine(file, rMessage, epi.getRows());
			}
			
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
	visualize();
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
