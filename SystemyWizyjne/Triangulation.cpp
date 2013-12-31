#include "Triangulation.h"

using namespace std;

void triangulation::triangulate(int pointsNumber, double* pointsList, string outputFile)
{
	triangulate(pointsNumber, pointsList, outputFile.c_str());
}

void triangulation::triangulate(int pointsNumber, double* pointsList, char* outputFile)
{
  tetgenio inp, out;
  tetgenbehavior behavior;
  behavior.parse_commandline("pq1.414a0.1");
 
  inp.initialize();
  out.initialize();
 
  inp.numberofpoints = pointsNumber;
  inp.pointlist = (REAL*) pointsList;

  tetrahedralize(&behavior, &inp, &out, 0, 0);
  out.save_poly(outputFile);
}