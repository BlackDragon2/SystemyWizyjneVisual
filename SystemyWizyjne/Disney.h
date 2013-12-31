#pragma once
#include "algorithm.h"
class Disney :
	public Algorithm
{
public:
	Disney(void);
	~Disney(void);
	void transform(double focal);
	void makeConsistent();
	void createFiles();
};

