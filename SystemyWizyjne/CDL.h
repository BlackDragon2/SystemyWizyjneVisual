#pragma once
#include "algorithm.h"
class CDL : public Algorithm
{
public:
	CDL(void);
	~CDL(void);
	void transform(double focal);
	void createFiles();
private:
	static const int PHI=1;
	static const int GAUSSIAN_ROWS=3;
	static const int GAUSSIAN_COLUMNS=3;
	static const GradientMask MASKX=GradientMask::ROBERTSX;
	static const GradientMask MASKY=GradientMask::ROBERTSY;

};

