#pragma once
#include "Utils.h"

enum GradientMask{SOBELX, PREWITTX, ROBERTSX, SCHARRX, SOBELY, PREWITTY, ROBERTSY, SCHARRY, STANDARDX, STANDARDY};

static double** getGradientMask(GradientMask mask)
{
	double** result=utils::create2DDouble(3,3);
	switch(mask)
	{
		case PREWITTX:
			result[0][0]=-1;
			result[0][1]=-1;
			result[0][2]=-1;
			result[1][0]=0;
			result[1][1]=0;
			result[1][2]=0;
			result[2][0]=1;
			result[2][1]=1;
			result[2][2]=1;
		break;
		case PREWITTY:
			result[0][0]=-1;
			result[0][1]=0;
			result[0][2]=1;
			result[1][0]=-1;
			result[1][1]=0;
			result[1][2]=1;
			result[2][0]=-1;
			result[2][1]=0;
			result[2][2]=1;
		break;
		case ROBERTSX:
			result[0][0]=0;
			result[0][1]=0;
			result[0][2]=0;
			result[1][0]=-1;
			result[1][1]=0;
			result[1][2]=0;
			result[2][0]=0;
			result[2][1]=1;
			result[2][2]=0;
		break;
		case ROBERTSY:
			result[0][0]=0;
			result[0][1]=0;
			result[0][2]=0;
			result[1][0]=0;
			result[1][1]=0;
			result[1][2]=-1;
			result[2][0]=0;
			result[2][1]=1;
			result[2][2]=0;
		break;
		case SOBELX:
			result[0][0]=-1;
			result[0][1]=-2;
			result[0][2]=-1;
			result[1][0]=0;
			result[1][1]=0;
			result[1][2]=0;
			result[2][0]=1;
			result[2][1]=2;
			result[2][2]=1;
		break;
		case SOBELY:
			result[0][0]=-1;
			result[0][1]=0;
			result[0][2]=1;
			result[1][0]=-2;
			result[1][1]=0;
			result[1][2]=2;
			result[2][0]=-1;
			result[2][1]=0;
			result[2][2]=1;
		break;
		case SCHARRX:
			result[0][0]=-3;
			result[0][1]=0;
			result[0][2]=3;
			result[1][0]=-10;
			result[1][1]=0;
			result[1][2]=10;
			result[2][0]=-3;
			result[2][1]=0;
			result[2][2]=3;
		break;
		case SCHARRY:
			result[0][0]=-3;
			result[0][1]=-10;
			result[0][2]=-3;
			result[1][0]=0;
			result[1][1]=0;
			result[1][2]=0;
			result[2][0]=3;
			result[2][1]=10;
			result[2][2]=3;
		break;
			case STANDARDX:
			result[0][0]=0;
			result[0][1]=0;
			result[0][2]=0;
			result[1][0]=-1;
			result[1][1]=0;
			result[1][2]=1;
			result[2][0]=0;
			result[2][1]=0;
			result[2][2]=0;
		break;
		case STANDARDY:
			result[0][0]=0;
			result[0][1]=-1;
			result[0][2]=0;
			result[1][0]=0;
			result[1][1]=0;
			result[1][2]=0;
			result[2][0]=0;
			result[2][1]=1;
			result[2][2]=0;
		break;	
	}
	return result;
}
