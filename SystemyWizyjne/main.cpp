#include <iostream>
#include <cstdlib>
#include "Algorithm.h"
#include "CDL.h"
#include "Position.h"

int main(int argc, const char* argv[])
{
	CDL cdl;
	if(argc>1)
	{
		Position position;
		if(*argv[2]=='H')
			position=HORIZONTAL;
		else
			if(*argv[2]=='V')
				position=VERTICAL;
			else
				position=BOTH;
		cdl.compute(argv[1], position, std::atof(argv[3]));
	}
	else
		cdl.compute("D:\\bikes\\config.txt", HORIZONTAL, 7800);
	system("pause");
}

