#include "Disney.h"


Disney::Disney(void)
{
}


Disney::~Disney(void)
{
}

void Disney::transform(double focal)
{
	if(getComputePosition()==HORIZONTAL||getComputePosition()==BOTH)
	{
		for(int i=0;i<getImageRows();i++)
		{
		}
	}
	if(getComputePosition()==VERTICAL||getComputePosition()==BOTH)
	{
		for(int i=0;i<getImageColumns();i++)
		{
		}
	}
}

void Disney::makeConsistent()
{
}
