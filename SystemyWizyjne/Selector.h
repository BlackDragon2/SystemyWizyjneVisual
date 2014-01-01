#pragma once
#include <vector>
#include <string>

namespace selector
{
	std::vector<double> select(std::string file);
	std::vector<double> select(char* file);
}