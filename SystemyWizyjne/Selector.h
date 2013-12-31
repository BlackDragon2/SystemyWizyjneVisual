#pragma once
#include <vector>
#include <string>

namespace selector
{
	std::vector<double> select(std::string config, int fileNr);
	std::vector<double> select(char* config, int fileNr);
}