#pragma once
#include "algorithm.h"
#define PHI 0.8
class CDL : public Algorithm
{
public:
	CDL(void);
	~CDL(void);
	void transform(double focal);
	void makeConsistent();
	void createFiles();
private:
static const int GAUSSIAN_ROWS=3;
static const int GAUSSIAN_COLUMNS=3;
static const GradientMask MASKX=GradientMask::SCHARRX;
static const GradientMask MASKY=GradientMask::SCHARRY;
};

