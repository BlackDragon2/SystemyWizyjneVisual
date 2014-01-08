#pragma once
#define THRESHOLD 0.02
#include "algorithm.h"
class Disney :
	public Algorithm
{
public:
	Disney(void);
	~Disney(void);
	void transform(double focal);
	void makeConsistent();
	double getCe(double* row, int length, int index, int size);

	const static int NEIGHBORS=9;
};

