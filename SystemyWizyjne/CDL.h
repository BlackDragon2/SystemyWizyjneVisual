#pragma once
#include "algorithm.h"
#define PHI 0.8
class CDL : public Algorithm
{
public:
	CDL(void);
	~CDL(void);
	void transform(double focal);
	void transformH(double focal, int groupID);
	void transformV(double focal, int groupID);
	void makeConsistent();
	void createFileH(int i, std::string type, bool del);
	void createFileV(int i, std::string type, bool del);
	void createFiles();
	void rotate();
private:
static const int GAUSSIAN_ROWS=3;
static const int GAUSSIAN_COLUMNS=3;
static const GradientMask MASKX=GradientMask::STANDARDX;
static const GradientMask MASKY=GradientMask::STANDARDY;
};

