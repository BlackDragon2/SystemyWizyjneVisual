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
	double** dX;
	double** dS;
	double** l;
	double** r;
	if(getComputePosition()==HORIZONTAL||getComputePosition()==BOTH)
	{
		for(int i=0;i<getImageRows();i++)
		{
			EPI epi(EPI::createEPIPath(getDir(), getBaseName(), i, HORIZONTAL));
			double** gradientX=graphicUtils::gradient(MASKX, epi.getPixels(), epi.getRows(), epi.getColumns());
			double** gradientY=graphicUtils::gradient(MASKY, epi.getPixels(), epi.getRows(), epi.getColumns());
			double** gaussian=graphicUtils::normalize(graphicUtils::GaussianKernel(PHI, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS), GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, true);
			Jxx=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientX, gradientX, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
			Jxy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientX, gradientY, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
			Jyy=graphicUtils::convolution(gaussian, GAUSSIAN_ROWS, GAUSSIAN_COLUMNS, graphicUtils::pixelMultiply(gradientY, gradientY, epi.getRows(), epi.getColumns()), epi.getRows(), epi.getColumns());
				
			//equation(5)
			dX=graphicUtils::pixelSubstract(Jyy, Jxx, epi.getRows(), epi.getColumns());
			dS=graphicUtils::pixelMultiply(2.0, Jxy, epi.getRows(), epi.getColumns());
			
				double[][] phi=MathUtilities.matrixMultiply(0.5, MathUtilities.arctan(GraphicUtilities.pixelDivide(dX, dS)));
				dX=MathUtilities.sin(phi);
				dS=MathUtilities.cos(phi);
				//equation(6)
				l=GraphicUtilities.pixelMultiply(-focalPoint, GraphicUtilities.pixelDivide(dS, dX));
				FileIO.saveArray(phi, new File("D:\\bikes\\test2.txt"));
		}
	}
	if(getComputePosition()==VERTICAL||getComputePosition()==BOTH)
	{
		for(int i=0;i<getImageColumns();i++)
		{
			EPI epi(EPI::createEPIPath(getDir(), getBaseName(), i, VERTICAL));
		}
	}
}
