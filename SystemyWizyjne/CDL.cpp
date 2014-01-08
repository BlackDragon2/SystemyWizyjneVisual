#include "CDL.h"

CDL::CDL(void) : Algorithm()
{
}


CDL::~CDL(void)
{
}

void CDL::rotate()
{
	for(int i=0;i<getVImages()*(getHImages()?getHImages():1);i++)
	{
		cout<<"Rotating "<<i+1<<" image from "<<getVImages()*(getHImages()?getHImages():1)<<endl;
		int rows;
		int cols;
		string file=getDir()+getBaseName()+"_image_"+to_string(i)+"_0.txt";
		string rFile=getDir()+getBaseName()+"_r_"+to_string(i)+"_0.txt";
		double** image=fileIO::loadArray(file, rows, cols);
		double** r=fileIO::loadArray(rFile, rows, cols);
		createFileH(i,"_0.txt", false);
		double* message=new double[rows];
		double* rMessage=new double[rows];
		for(int j=0;j<cols;j++)
		{
			for(int k=0;k<rows;k++)
			{
				message[k]=image[k][j];
				rMessage[k]=r[k][j];
			}
			fileIO::saveLine(file, message, rows);
			fileIO::saveLine(rFile, rMessage, rows);
		}
		delete[] message;
		delete[] rMessage;
		utils::memFree(image, rows);
		utils::memFree(r, rows);
	}
}

void CDL::makeConsistent()
{
	int rows=0;
	int cols=0;
	for(int i=0;i<getHImages()*getVImages();i++)
	{
		cout<<"Joining image "<<i<<" of "<<getHImages()*getVImages()<<endl;
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
		fileIO::saveArray(getDir()+getBaseName()+"_final_"+to_string(i)+".txt", result, rows, cols, false);
		fileIO::saveArray(getDir()+getBaseName()+"_finalR_"+to_string(i)+".txt", result, rows, cols, false);
		utils::memFree(hor, rows);
		utils::memFree(ver, rows);
		utils::memFree(horR, rows);
		utils::memFree(verR, rows);
		utils::memFree(result, rows);
		utils::memFree(resultR, rows);
	}
	cout<<"Joining done"<<endl;
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
	if(getComputePosition()==BOTH)
	{
		for(int i=1;i<=getVImages();i++)
			transformH(focal, i);
		for(int i=1;i<=getHImages();i++)
			transformV(focal, i);
		rotate();
	}
	if(getComputePosition()==HORIZONTAL)
	{	
		transformH(focal, 1);
	}
	if(getComputePosition()==VERTICAL)
	{	
		transformV(focal, 1);
		rotate();
	}
	if(getComputePosition()==BOTH)
		makeConsistent();
}

void CDL::transformH(double focal, int groupID)
{
	double** Jxx;
	double** Jxy;
	double** Jyy;
	double** r;

	double** gaussian=graphicUtils::normalize(graphicUtils::GaussianKernel(PHI, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS), GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, true);
	for(int i=0;i<getImageRows();i++)
	{
		EPI epi(EPI::createEPIPath(getDir(), groupID, getBaseName(), i, HORIZONTAL));
		cout<<"Procesing "<<getImageRows()*(groupID-1)+i+1<<" EPI from "<<getImageRows()*(getVImages()?getVImages():1)<<endl;
		//from [0-255] range to [0-1]
		double** tempPixels=graphicUtils::toGrayScale(epi.getPixels(), epi.getRows(), epi.getColumns(), GrayMethod::LIGHTNESS);
		double** pixels=graphicUtils::pixelMultiply(1.0/255.0, tempPixels, epi.getRows(), epi.getColumns()); 
	
		double** gradientX=graphicUtils::gradient(MASKX, pixels, epi.getRows(), epi.getColumns());
		double** gradientY=graphicUtils::gradient(MASKY, pixels, epi.getRows(), epi.getColumns());

		utils::memFree(pixels, epi.getRows());
		utils::memFree(tempPixels, epi.getRows());

		double** xx=graphicUtils::pixelMultiply(gradientX, gradientX, epi.getRows(), epi.getColumns());
		double** xy=graphicUtils::pixelMultiply(gradientX, gradientY, epi.getRows(), epi.getColumns());
		double** yy=graphicUtils::pixelMultiply(gradientY, gradientY, epi.getRows(), epi.getColumns());

		utils::memFree(gradientX, epi.getRows());
		utils::memFree(gradientY, epi.getRows());
			
		Jxx=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, xx, epi.getRows(), epi.getColumns());
		Jxy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, xy, epi.getRows(), epi.getColumns());
		Jyy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, yy, epi.getRows(), epi.getColumns());

		utils::memFree(xx, epi.getRows());
		utils::memFree(xy, epi.getRows());
		utils::memFree(yy, epi.getRows());

		//equation(5)
		double** sub=graphicUtils::pixelSubstract(Jyy, Jxx, epi.getRows(), epi.getColumns());
		double** mul=graphicUtils::pixelMultiply(2.0, Jxy, epi.getRows(), epi.getColumns());
		double** arctan=mathUtils::arctan(mul, sub, epi.getRows(), epi.getColumns());
		double** phi=graphicUtils::pixelMultiply(0.5, arctan, epi.getRows(), epi.getColumns());
		//equation(7)
		double** sum=graphicUtils::pixelAdd(Jxx, Jyy, epi.getRows(), epi.getColumns());

		utils::memFree(Jxx, epi.getRows());
		utils::memFree(Jxy, epi.getRows());
		utils::memFree(Jyy, epi.getRows());

		double** sumQuater=graphicUtils::pixelMultiply(sum, sum, epi.getRows(), epi.getColumns());

		double** subQuater=graphicUtils::pixelMultiply(sub, sub, epi.getRows(), epi.getColumns());
		double** mulQuater=graphicUtils::pixelMultiply(mul, mul, epi.getRows(), epi.getColumns());
		double** subPlusMul=graphicUtils::pixelAdd(subQuater, mulQuater, epi.getRows(), epi.getColumns());

		r=graphicUtils::pixelDivide(subPlusMul, sumQuater, epi.getRows(), epi.getColumns());
		double* message=new double[epi.getColumns()];
		for(int k=0;k<epi.getRows();k++)
		{			
			for(int j=0;j<epi.getColumns();j++)
			{
				if(phi[k][j]<0)
					phi[k][j]=-phi[k][j];
				if(phi[k][j]==0)
				{
					if(j>0)
						phi[k][j]=phi[k][j-1];
					else
					{
						int l=1;
						while(l<epi.getColumns()&&!(phi[k][j]=phi[k][j+l]))
							l++;
					}
				}
				message[j]=focal/tan(phi[k][j]);
			}
			string file=getDir()+getBaseName()+"_image_"+to_string(k+(groupID-1)*getHImages())+"_1.txt";
			fileIO::saveLine(file, message, epi.getColumns());
			file=getDir()+getBaseName()+"_r_"+to_string(k+(groupID-1)*getHImages())+"_1.txt";
			fileIO::saveLine(file, r[k], epi.getColumns());
		}
		delete[] message;
		utils::memFree(phi, epi.getRows());
		utils::memFree(r, epi.getRows());
		utils::memFree(sub, epi.getRows());
		utils::memFree(mul, epi.getRows());
		utils::memFree(sum, epi.getRows());
		utils::memFree(subQuater, epi.getRows());
		utils::memFree(mulQuater, epi.getRows());
		utils::memFree(sumQuater, epi.getRows());
		utils::memFree(subPlusMul, epi.getRows());
		utils::memFree(arctan, epi.getRows());
	}
	utils::memFree(gaussian, 3);
}

void CDL::transformV(double focal, int groupID)
{
	double** Jxx;
	double** Jxy;
	double** Jyy;
	double** r;

	double** gaussian=graphicUtils::normalize(graphicUtils::GaussianKernel(PHI, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS), GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, true);
	for(int i=0;i<getImageColumns();i++)
	{
		EPI epi(EPI::createEPIPath(getDir(), groupID, getBaseName(), i, VERTICAL));
		cout<<"Procesing "<<getImageRows()*(groupID-1)+i+1<<" EPI from "<<getImageColumns()*(getHImages()?getHImages():1)<<endl;
		//from [0-255] range to [0-1]
		double** tempPixels=graphicUtils::toGrayScale(epi.getPixels(), epi.getRows(), epi.getColumns(), GrayMethod::LIGHTNESS);
		double** pixels=graphicUtils::pixelMultiply(1.0/255.0, tempPixels, epi.getRows(), epi.getColumns()); 
	
		double** gradientX=graphicUtils::gradient(MASKX, pixels, epi.getRows(), epi.getColumns());
		double** gradientY=graphicUtils::gradient(MASKY, pixels, epi.getRows(), epi.getColumns());

		utils::memFree(pixels, epi.getRows());
		utils::memFree(tempPixels, epi.getRows());

		double** xx=graphicUtils::pixelMultiply(gradientX, gradientX, epi.getRows(), epi.getColumns());
		double** xy=graphicUtils::pixelMultiply(gradientX, gradientY, epi.getRows(), epi.getColumns());
		double** yy=graphicUtils::pixelMultiply(gradientY, gradientY, epi.getRows(), epi.getColumns());

		utils::memFree(gradientX, epi.getRows());
		utils::memFree(gradientY, epi.getRows());
			
		Jxx=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, xx, epi.getRows(), epi.getColumns());
		Jxy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, xy, epi.getRows(), epi.getColumns());
		Jyy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, yy, epi.getRows(), epi.getColumns());

		utils::memFree(xx, epi.getRows());
		utils::memFree(xy, epi.getRows());
		utils::memFree(yy, epi.getRows());

		//equation(5)
		double** sub=graphicUtils::pixelSubstract(Jyy, Jxx, epi.getRows(), epi.getColumns());
		double** mul=graphicUtils::pixelMultiply(2.0, Jxy, epi.getRows(), epi.getColumns());
		double** arctan=mathUtils::arctan(mul, sub, epi.getRows(), epi.getColumns());
		double** phi=graphicUtils::pixelMultiply(0.5, arctan, epi.getRows(), epi.getColumns());
		//equation(7)
		double** sum=graphicUtils::pixelAdd(Jxx, Jyy, epi.getRows(), epi.getColumns());

		utils::memFree(Jxx, epi.getRows());
		utils::memFree(Jxy, epi.getRows());
		utils::memFree(Jyy, epi.getRows());

		double** sumQuater=graphicUtils::pixelMultiply(sum, sum, epi.getRows(), epi.getColumns());

		double** subQuater=graphicUtils::pixelMultiply(sub, sub, epi.getRows(), epi.getColumns());
		double** mulQuater=graphicUtils::pixelMultiply(mul, mul, epi.getRows(), epi.getColumns());
		double** subPlusMul=graphicUtils::pixelAdd(subQuater, mulQuater, epi.getRows(), epi.getColumns());

		r=graphicUtils::pixelDivide(subPlusMul, sumQuater, epi.getRows(), epi.getColumns());
		double* message=new double[epi.getColumns()];
		for(int k=0;k<epi.getRows();k++)
		{			
			for(int j=0;j<epi.getColumns();j++)
			{
				if(phi[k][j]<0)
					phi[k][j]=-phi[k][j];
				if(phi[k][j]==0)
				{
					if(j>0)
						phi[k][j]=phi[k][j-1];
					else
					{
						int l=1;
						while(l<epi.getColumns()&&!(phi[k][j]=phi[k][j+l]))
							l++;
					}
				}
				message[j]=focal/tan(phi[k][j]);
			}
			string file=getDir()+getBaseName()+"_image_"+to_string(k+(groupID-1)*getHImages())+"_0.txt";
			fileIO::saveLine(file, message, epi.getColumns());
			file=getDir()+getBaseName()+"_r_"+to_string(k+(groupID-1)*getHImages())+"_0.txt";
			fileIO::saveLine(file, r[k], epi.getColumns());
		}
		delete[] message;
		utils::memFree(phi, epi.getRows());
		utils::memFree(r, epi.getRows());
		utils::memFree(sub, epi.getRows());
		utils::memFree(mul, epi.getRows());
		utils::memFree(sum, epi.getRows());
		utils::memFree(subQuater, epi.getRows());
		utils::memFree(mulQuater, epi.getRows());
		utils::memFree(sumQuater, epi.getRows());
		utils::memFree(subPlusMul, epi.getRows());
		utils::memFree(arctan, epi.getRows());
	}
	utils::memFree(gaussian, 3);
}

void CDL::createFiles()
{
	if(getComputePosition()==BOTH)
	{
		for(int i=0;i<getHImages()*getVImages();i++)
		{
			createFileH(i,"_1.txt", true);
			createFileV(i,"_0.txt", false);
		}
	}
	if(getComputePosition()==HORIZONTAL)
	{
		for(int i=0;i<getHImages();i++)
			createFileH(i,"_1.txt", true);
	}
	if(getComputePosition()==VERTICAL)
	{
		for(int i=0;i<getVImages();i++)
			createFileV(i,"_0.txt", true);
	}
}

void CDL::createFileH(int i, string type, bool del)
{	
	string file=getDir()+getBaseName()+"_image_"+to_string(i)+type;
	remove(file.c_str());
	string message=to_string(getImageRows())+"\n"+to_string(getImageColumns());
	fileIO::saveLine<string>(file, message);
	file=getDir()+getBaseName()+"_r_"+to_string(i)+type;
	remove(file.c_str());
	message=to_string(getImageRows())+"\n"+to_string(getImageColumns());
	fileIO::saveLine<string>(file, message);
	if(del)
	{
		string file=getDir()+getBaseName()+"_final_"+to_string(i)+".txt";
		remove(file.c_str());
		file=getDir()+getBaseName()+"_finalR_"+to_string(i)+".txt";
		remove(file.c_str());
	}
}

void CDL::createFileV(int i, string type, bool del)
{	
	string file=getDir()+getBaseName()+"_image_"+to_string(i)+type;
	remove(file.c_str());
	string message=to_string(getImageColumns())+"\n"+to_string(getImageRows());
	fileIO::saveLine<string>(file, message);
	file=getDir()+getBaseName()+"_r_"+to_string(i)+type;
	remove(file.c_str());
	message=to_string(getImageColumns())+"\n"+to_string(getImageRows());
	fileIO::saveLine<string>(file, message);
	if(del)
	{
		string file=getDir()+getBaseName()+"_final_"+to_string(i)+".txt";
		remove(file.c_str());
		file=getDir()+getBaseName()+"_finalR_"+to_string(i)+".txt";
		remove(file.c_str());
	}
}
