#pragma once
#include "EPI.h"
#include "GraphicUtils.h"
#include "GrayMethod.h"


class Algorithm
{

public:
	Algorithm(void);
	virtual ~Algorithm(void);
	virtual void transform(double focal)=0;
	bool EPIExists(std::string config);
	bool EPIExists(const char* config);
	void compute(std::string config, Position position, double focalPoint);
	void createEPI(string config);

	std::string getDir() { return dir_; };
	std::string getBaseName() {	return baseName_; };
	std::string getExtension() { return extension_;	};
	int getHImages() { return hImages_; };
	int getVImages() { return vImages_; };
	int getImageRows() { return imageRows_; };
	int getImageColumns() { return imageColumns_; };
	Position getComputePosition() { return computePosition_; };

	void setDir(std::string dir) { dir_=dir; };
	void setBaseName(std::string baseName) { baseName_=baseName; };
	void setExtension(std::string extension) { extension_=extension; };
	void setHImages(int hImages) { hImages_=hImages; };
	void setVImages(int vImages) { vImages_=vImages; };
	void setImageRows(int imageRows) { imageRows_=imageRows; };
	void setImageColumns(int imageColumns) { imageColumns_=imageColumns; };
	void setComputePosition(Position computePosition) { computePosition_=computePosition; };

private:
	std::string dir_;
	std::string baseName_;
	std::string extension_;
	int hImages_;
	int vImages_;
	int imageRows_;
	int imageColumns_;
	Position computePosition_;
};

