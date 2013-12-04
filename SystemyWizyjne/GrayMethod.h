#pragma once
#include <algorithm>

enum GrayMethod{LIGHTNESS, LUMINOSITY, AVERAGE};

inline double maximum(double a, double b, double c)
{
	return std::max(std::max(a,b),c);
};

inline double minimum(double a, double b, double c)
{
	return std::min(std::min(a,b),c);
};

int toGray(GrayMethod m, int r, int g, int b);




