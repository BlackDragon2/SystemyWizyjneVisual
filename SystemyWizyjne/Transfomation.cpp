#include "Transfomation.h"

void transformation::transform(std::vector<double> in, double xAngle, double yAngle, double zAngle)
{
	for(int i=0;i<in.size()/3;i++)
		transform(&in[3*i], &in[3*i+1], &in[3*i+2], xAngle, yAngle, zAngle);
}

void transformation::transform(double* x, double* y, double* z, double xAngle, double yAngle, double zAngle)
{
	double xRads=toRads(xAngle);
	double yRads=toRads(yAngle);
	double zRads=toRads(zAngle);

	*x=(cos(yRads)*(cos(zRads)+sin(zRads))-sin(yRads))*(*x);
	*y=(sin(xRads)*(sin(yRads)*(cos(zRads)+sin(zRads))+cos(yRads))+cos(xRads)*(cos(zRads)-sin(zRads)))*(*y);
	*z=(cos(xRads)*(sin(yRads)*(cos(zRads)+sin(zRads))+cos(yRads))+sin(xRads)*(sin(zRads)-cos(zRads)))*(*z);
}


