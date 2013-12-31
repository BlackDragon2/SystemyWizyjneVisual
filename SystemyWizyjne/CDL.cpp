#include "CDL.h"

CDL::CDL(void) : Algorithm()
{
}


CDL::~CDL(void)
{
}

void visualize2(double** arr=0, int rows=0, int cols=0)
{
	if(arr==0)
		arr=fileIO::loadArray("D:\\bikes\\bikes_r_26_1.txt", rows, cols);
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
	cout<<"Min "<<min<<endl;
	cout<<"Max "<<max<<endl;
	if(max>1)
		max=1;
	if(min<0)
		min=0;
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			if(arr[i][j]>max)
				arr[i][j]=max;
			if(arr[i][j]<min)
				arr[i][j]=min;
			arr[i][j]=((arr[i][j]-min)/(max-min))*255;//white most reliable
		}
	}
	graphicIO::saveImage(arr, rows, cols, "D:\\other\\rel_26.jpg", 1);
}

void visualize(double** arr=0, int rows=0, int cols=0)
{
	if(arr==0)
		arr=fileIO::loadArray("D:\\bikes\\bikes_image_26_1.txt", rows, cols);
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
	cout<<"Min "<<min<<endl;
	cout<<"Max "<<max<<endl;
	if(max>1500)
		max=1500;
	if(min<100)
		min=100;
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			if(arr[i][j]>max)
				arr[i][j]=max;
			if(arr[i][j]<min)
				arr[i][j]=min;
			arr[i][j]=255-((arr[i][j]-min)/(max-min))*255;//255-... so that black(0,0,0) corresponds to greatest distance
		}
	}
	graphicIO::saveImage(arr, rows, cols, "D:\\other\\final.jpg", 1);
}

void CDL::makeConsistent()
{
	int rows=0;
	int cols=0;
	for(int i=0;i<getHImages();i++)
	{
		double** ver=fileIO::loadArray(getDir()+getBaseName()+"_image_"+to_string(i)+"_0.txt", rows, cols);
		double** hor=fileIO::loadArray(getDir()+getBaseName()+"_image_"+to_string(i)+"_1.txt", rows, cols);
		double** verR=fileIO::loadArray(getDir()+getBaseName()+"_r_"+to_string(i)+"_0.txt", rows, cols);
		double** horR=fileIO::loadArray(getDir()+getBaseName()+"_r_"+to_string(i)+"_1.txt", rows, cols);
		double** result=utils::create2DDouble(rows, cols);
		double** resultR=utils::create2DDouble(rows, cols);
		for(int r=0;r<rows;r++)
		{
			for(int c=0;c<cols;c++)
			{
				if(verR[r][c]<horR[r][c])
				{
					result[r][c]=hor[r][c];
					resultR[r][c]=horR[r][c];
				}
				else
				{
					result[r][c]=ver[r][c];
					resultR[r][c]=verR[r][c];
				}
			}
		}
		fileIO::saveArray("D:\\bikes\\bikes_final_"+to_string(i)+".txt", result, rows, cols, false);
		fileIO::saveArray("D:\\bikes\\bikes_finalR_"+to_string(i)+".txt", result, rows, cols, false);
		utils::memFree(hor, rows);
		utils::memFree(ver, rows);
		utils::memFree(horR, rows);
		utils::memFree(verR, rows);
		utils::memFree(result, rows);
		utils::memFree(resultR, rows);
	}
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

	createFiles();

	if(getComputePosition()==HORIZONTAL||getComputePosition()==BOTH)
	{
		//EPI epi(EPI::createEPIPath(getDir(), getBaseName(), 0, HORIZONTAL));
		//graphicIO::showImage(epi.getPixels(), epi.getRows(), epi.getColumns(), 1);
		
		double** gaussian=graphicUtils::normalize(graphicUtils::GaussianKernel(PHI, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS), GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, true);
		for(int i=0;i<getImageRows();i++)
		{
			cout<<"Procesing "<<i+1<<" EPI from "<<getImageRows()<<endl;
			EPI epi(EPI::createEPIPath(getDir(), getBaseName(), i, HORIZONTAL));
			//from [0-255] range to [0-1]
			double** tempPixels=graphicUtils::toGrayScale(epi.getPixels(), epi.getRows(), epi.getColumns(), GrayMethod::LIGHTNESS);
			double** pixels=graphicUtils::pixelMultiply(1.0/255.0, tempPixels, epi.getRows(), epi.getColumns()); 
	
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
			//double** arctan=mathUtils::arctan(mul, sub, epi.getRows(), epi.getColumns());
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
				{
					if(phi[i][j]<0)
						phi[i][j]=-phi[i][j];
					if(phi[i][j]==0)
					{
						if(j>0)
							phi[i][j]=phi[i][j-1];
						else
						{
							int k=1;
							while(k<epi.getColumns()&&!(phi[i][j]=phi[i][j+k]))
								k++;
						}
					}
					message[j]=focal/(tan(phi[i][j]));
				}//message[j]=-focal*(tan(phi[i][j]))*255;
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
			utils::memFree(tempPixels, epi.getRows());
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
	if(getComputePosition()==VERTICAL||getComputePosition()==BOTH)
	{
		for(int i=0;i<getImageColumns();i++)
		{
			EPI epi(EPI::createEPIPath(getDir(), getBaseName(), i, VERTICAL));

			//from [0-255] range to [0-1]
			double** tempPixels=graphicUtils::toGrayScale(epi.getPixels(), epi.getRows(), epi.getColumns(), GrayMethod::LIGHTNESS);
			double** pixels=graphicUtils::pixelMultiply(1.0/255.0, tempPixels, epi.getRows(), epi.getColumns()); 

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
			utils::memFree(tempPixels, epi.getRows());
			utils::memFree(gradientX, epi.getRows());
			utils::memFree(gradientY, epi.getRows());
			utils::memFree(gaussian, epi.getRows());
			utils::memFree(Jxx, epi.getRows());
			utils::memFree(Jxy, epi.getRows());
			utils::memFree(Jyy, epi.getRows());
		}
	}
	if(getComputePosition()==BOTH)
		makeConsistent();
	visualize();
	visualize2();
}

void CDL::createFiles()
{
	if(getComputePosition()==HORIZONTAL||getComputePosition()==BOTH)
	{
		for(int i=0;i<getHImages();i++)
		{
			string file=getDir()+getBaseName()+"_image_"+utils::to_string<int>(i)+"_1.txt";
			remove(file.c_str());
			string message=utils::to_string<int>(getImageRows())+"\n"+utils::to_string<int>(getImageColumns());
			fileIO::saveLine<string>(file, message);
			file=getDir()+getBaseName()+"_r_"+utils::to_string<int>(i)+"_1.txt";
			remove(file.c_str());
			message=utils::to_string<int>(getImageRows())+"\n"+utils::to_string<int>(getImageColumns());
			fileIO::saveLine<string>(file, message);
		}
	}
	if(getComputePosition()==VERTICAL||getComputePosition()==BOTH)
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
