#pragma once
#include "Utils.h"
#include "tetgen.h"

namespace triangulation
{
	/*
	* pointList is normal array with elements as follow x1,y1,z1,x2,y2,...,xn,yn,zn so it's is 3xpointNumber
	* outFile must be should be in ply format
	*/
	void triangulate(int pointsNumber, double* pointsList, char* outFile);
	void triangulate(int pointsNumber, double* pointsList, std::string outFile);
}