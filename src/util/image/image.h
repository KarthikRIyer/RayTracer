#pragma once
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "util/stb/stb_image_write.h"
#include "util/stb/stb_image.h"
#include "util/helper/helper.h"
#include <iostream>
#include <string>

class Image
{
public:
	Image(int width, int height) {
		this->width = width;
		this->height = height;
		imgBuffer = new float[width * height * nrChannels];
	}

	Image(std::string path) {
		imgBuffer = stbi_loadf(path.c_str(), &width, &height, &nrChannels, 0);
	}

	~Image() {
		delete imgBuffer;
	}

	float& operator [] (int i) {
		return imgBuffer[i];
	}

	float* getBuffer() {
		return imgBuffer;
	}

	void getCharBuffer(unsigned char* buffer) {
		for (unsigned int i = 0; i < (width * height * nrChannels); i++)
		{
			buffer[i] = clamp(255.99 * imgBuffer[i], 0.0, 255.0);
		}
	}

	int getChannels() {
		return nrChannels;
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	void saveImage(std::string path) {
		int index = path.find_last_of('.');
		std::string fileType = path.substr(index + 1);
		if (fileType == "jpg") {
			writeJPG(path);
		}
		else if (fileType == "png") {
			writePNG(path);
		}
		else if (fileType == "bmp") {
			writeBMP(path);
		}
		else if (fileType == "hdr") {
			writeHDR(path);
		}
		else {
			std::cout << "File Write Error::Unknown File Type. Writing PNG.\n";
			writePNG(path);
		}
	}
	

private:
	float* imgBuffer;
	int width;
	int height;
	int nrChannels = 3;

	void writeJPG(std::string path) {
		unsigned char* charBuffer = new unsigned char[width * height * nrChannels];
		getCharBuffer(charBuffer);
		stbi_write_jpg(path.c_str(), width, height, nrChannels, charBuffer, 100);
		delete[] charBuffer;
	}

	void writePNG(std::string path) {
		unsigned char* charBuffer = new unsigned char[width*height*nrChannels];
		getCharBuffer(charBuffer);
		stbi_write_png(path.c_str(), width, height, nrChannels, charBuffer, width * nrChannels);
		delete[] charBuffer;
	}

	void writeBMP(std::string path) {
		unsigned char* charBuffer = new unsigned char[width * height * nrChannels];;
		getCharBuffer(charBuffer);
		stbi_write_bmp(path.c_str(), width, height, nrChannels, charBuffer);
		delete[] charBuffer;
	}

	void writeHDR(std::string path) {
		stbi_write_hdr(path.c_str(), width, height, nrChannels, imgBuffer);
	}
};
