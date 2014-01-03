#include "EPI.h"

EPI::EPI(Position positon, double** pixels, int line, int rows, int columns):position_(positon), pixels_(pixels), line_(line), rows_(rows), columns_(columns)
{
}

EPI::EPI(string path)
{
	loadEPI(path.c_str(), (int)path.length());
}

EPI::EPI(const char* path, int size)
{
	loadEPI(path, size);
}

EPI::EPI(EPI &epi)
{
	position_=epi.position_;
	line_=epi.line_;
	rows_=epi.rows_;
	columns_=epi.columns_;
	pixels_=utils::create2DDouble(rows_, columns_);
	for(int i=0;i<rows_;i++)
		memcpy(pixels_[i], epi.pixels_[i], columns_*sizeof(double));
}


EPI::~EPI(void)
{
	utils::memFree(pixels_, rows_);
}

void EPI::loadEPI(std::string path)
{
	loadEPI(path.c_str(), (int)path.length());
};

void EPI::loadEPI(const char* path, int size)
{
	const char* pointer=&path[size-6];//...1.txt0
	int help=(*pointer)-'0';
	if(help)
		position_=HORIZONTAL;
	else
		position_=VERTICAL;
	pointer-=2;
	help=0;
	while(*pointer!='_')
	{
		help*=10;
		help+=*pointer;
		pointer--;
	}
	line_=help;
	if(position_==HORIZONTAL)
		pixels_=fileIO::loadArray(path, rows_, columns_);
	else
	{
		double** temp=fileIO::loadArray(path, columns_, rows_);
		pixels_=utils::create2DDouble(rows_, columns_);
		for(int i=0;i<rows_;i++)
			for(int j=0;j<columns_;j++)
				pixels_[i][j]=temp[j][i];
		utils::memFree(temp, columns_);
	}
};

void EPI::saveEPI(const char* path)
{
	fileIO::saveArray(path, pixels_, rows_, columns_, position_==VERTICAL);
};

void EPI::saveEPI(std::string path)
{
	fileIO::saveArray(path, pixels_, rows_, columns_, position_==VERTICAL);
};

/*static*/ std::string EPI::createEPIPath(std::string dir, int groupID,  std::string name, int line, Position position)
{
	if(position==HORIZONTAL)
		return dir+to_string(groupID)+name+"_"+to_string(line)+"_1.txt";
	else
		return dir+to_string(groupID)+name+"_"+to_string(line)+"_0.txt";
};

void EPI::print(bool color)
{
	if(color)
		graphicIO::showImage(graphicIO::createImage(mathUtils::DoubleToInt(pixels_, rows_, columns_), rows_, columns_));
	else
		graphicIO::showImage(graphicIO::createImage(mathUtils::DoubleToInt(pixels_, rows_, columns_), rows_, columns_, 1));
};
