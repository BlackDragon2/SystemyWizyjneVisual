#pragma once
#include <vector>
#include <string>
#include "FileIO.h"
#include "Utils.h"

namespace selector
{
	std::vector<double> select(std::string file, double min, double max, double dist);
	std::vector<double> select(char* file, double min, double max, double dist);
}