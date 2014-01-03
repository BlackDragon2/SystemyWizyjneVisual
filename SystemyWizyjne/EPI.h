#pragma once
#include "Position.h"
#include "Utils.h"
#include "FileIO.h"
#include "GraphicIO.h"
#include "MathUtils.h"

class EPI
{
public:
	EPI(Position positon, double** pixels, int line, int rows, int columns);
	EPI(std::string path);
	EPI(const char* path, int size);
	EPI(EPI &epi);
	void loadEPI(std::string path);
	void loadEPI(const char* path, int size);
	void saveEPI(const char* path);	
	void saveEPI(std::string path);
	static std::string createEPIPath(std::string dir, int groupID, std::string name, int line, Position position);
	void print(bool color);
	~EPI(void);
	inline double** getPixels()
	{
		return pixels_;
	};
	inline Position getPosition()
	{
		return position_;
	};
	inline int getLine()
	{
		return line_;
	};
	inline int getRows()
	{
		return rows_;
	};
	inline int getColumns()
	{
		return columns_;
	};
	inline void setPixels(double** pixels)
	{
		pixels_=pixels;
	};
	inline void setPosition(Position position)
	{
		position_=position;
	};
	inline void setLine(int line)
	{
		line_=line;
	};
	inline void setRows(int rows)
	{
		rows_=rows;
	};
	inline void setColumns(int columns)
	{
		columns_=columns;
	};
private:	
	Position position_;
	double** pixels_;
	int line_;
	int rows_;
	int columns_;
};

