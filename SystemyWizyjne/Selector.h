#pragma once
#define LAST 3
#include <vector>
#include <string>
#include "FileIO.h"
#include "Utils.h"
#include "GraphicIO.h"
#include "GraphicUtils.h"
#include "GrayMethod.h"

namespace selector
{
	std::vector<double> select(std::string file, std::string source, double minZ, double maxZ, double minX, double maxX, double minY, double maxY);
	std::vector<double> select(char* file, char* source, double minZ, double maxZ, double minX, double maxX, double minY, double maxY);
}