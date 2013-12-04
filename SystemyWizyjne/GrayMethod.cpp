#include "GrayMethod.h"
int toGray(GrayMethod m, int r, int g, int b)
{
	double localResult;
	switch (m)
	{
	case LIGHTNESS:
		localResult=(maximum(r,g,b)+minimum(r,b,g))/2;
		break;
	case LUMINOSITY:
		localResult= 0.21*r + 0.71*g + 0.07*b;
		break;
	case AVERAGE:
		localResult=(r+g+b)/3;
		break;
	default:
		localResult=0;
		break;
	}
	return localResult;
};
