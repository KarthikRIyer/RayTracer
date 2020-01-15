#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "image.h"
#include "util/stb/stb_image_write.h"
#include "util/stb/stb_image.h"

Image::Image(int width, int height) {
	this->width = width;
	this->height = height;
	imgBuffer = new float[width * height * nrChannels];
}

Image::Image(std::string path) {
	imgBuffer = stbi_loadf(path.c_str(), &width, &height, &nrChannels, 0);
}

float& Image::operator [] (int i) {
	return imgBuffer[i];
}

void Image::clearBuffer() {
	delete[] imgBuffer;
	width = height = 0;
	nrChannels = 3;
}

float* Image::getBuffer() {
	return imgBuffer;
}

void Image::getCharBuffer(unsigned char* buffer) {
	for (unsigned int i = 0; i < (width * height * nrChannels); i++)
	{
		buffer[i] = clamp(255.99 * imgBuffer[i], 0.0, 255.0);
	}
}

int Image::getChannels() {
	return nrChannels;
}

int Image::getWidth() {
	return width;
}

int Image::getHeight() {
	return height;
}

void Image::setBuffer(float* buffer, int w, int h) {
	imgBuffer = buffer;
	width = w;
	height = h;
}

void Image::setBuffer(unsigned char* buffer, int w, int h) {
	width = w;
	height = h;
	nrChannels = 3;
	for (unsigned int i = 0; i < (width * height * 3); i++)
	{
		imgBuffer[i] = buffer[i] / 255.99;
	}
}

void Image::saveImage(std::string path) {
	int index = path.find_last_of('.');
	fileType = path.substr(index + 1);
	imagePath = path.substr(0, index + 1);
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
		std::cout << "File Write Error::Unknown File Type.\n";
		if (imagePath.size() > 0)
		{
			fileType = "png";
			std::cout << "Writing PNG.\n";
			writePNG(path);
		}
	}
}

void Image::writeJPG(std::string path) {
	unsigned char* charBuffer = new unsigned char[width * height * nrChannels];
	getCharBuffer(charBuffer);
	stbi_write_jpg(path.c_str(), width, height, nrChannels, charBuffer, 100);
	std::cout << "Saved Image: " << imagePath << fileType << std::endl;
	delete[] charBuffer;
}

void Image::writePNG(std::string path) {
	unsigned char* charBuffer = new unsigned char[width * height * nrChannels];
	getCharBuffer(charBuffer);
	stbi_write_png(path.c_str(), width, height, nrChannels, charBuffer, width * nrChannels);
	std::cout << "Saved Image: " << imagePath << fileType << std::endl;
	delete[] charBuffer;
}

void Image::writeBMP(std::string path) {
	unsigned char* charBuffer = new unsigned char[width * height * nrChannels];;
	getCharBuffer(charBuffer);
	stbi_write_bmp(path.c_str(), width, height, nrChannels, charBuffer);
	std::cout << "Saved Image: " << imagePath << fileType << std::endl;
	delete[] charBuffer;
}

void Image::writeHDR(std::string path) {
	stbi_write_hdr(path.c_str(), width, height, nrChannels, imgBuffer);
	std::cout << "Saved Image: " << imagePath << fileType << std::endl;
}