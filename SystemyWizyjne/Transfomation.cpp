#include "Transfomation.h"

using namespace std;

void transformation::transform(std::vector<double> in, double xAngle, double yAngle, double zAngle)
{
	cout<<"Rotating selected point"<<endl;
	for(int i=0;i<in.size()/3;i++)
		transform(&in[3*i], &in[3*i+1], &in[3*i+2], xAngle, yAngle, zAngle);
	cout<<"Rotation done"<<endl;
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


