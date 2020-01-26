#pragma once
#include "util/helper/helper.hpp"
#include <iostream>
#include <string>

class Image
{
public:
	Image() {}
	Image(int width, int height);
	Image(std::string path);

	~Image() {
		delete imgBuffer;
	}

	float& operator [] (int i);
	float* getBuffer();
	void getCharBuffer(unsigned char* buffer);
	int getChannels();
	int getWidth();
	int getHeight();
	void saveImage(std::string path);
	void setBuffer(float* buffer, int w, int h);
	void setBuffer(unsigned char* buffer, int w, int h);
	void clearBuffer();	

private:
	float* imgBuffer = NULL;
	int width = 0;
	int height = 0;
	int nrChannels = 3;
	std::string imagePath = "";
	std::string fileType = "";

	void writeJPG(std::string path);
	void writePNG(std::string path);
	void writeBMP(std::string path);
	void writeHDR(std::string path);
};
