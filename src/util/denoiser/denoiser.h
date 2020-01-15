#pragma once
#include <iostream>
#include <OpenImageDenoise/oidn.hpp>

class Denoiser {
public:
	Denoiser(float* imageInput,
		     float* imageOutput,
		     int width,
		     int height,
		     int nrChannels,
		     float* albedoInput = NULL,
		     float* normalInput = NULL,
		     bool isHDR = false,
		     bool isSRGB = true) {
		this->imageInput = imageInput;
		this->albedoInput = albedoInput;
		this->normalInput = normalInput;
		this->imageOutput = imageOutput;
		this->height = height;
		this->width = width;
		this->nrChannels = nrChannels;
		this->isHDR = isHDR;
		this->isSRGB = isSRGB;
	}

	void executeDenoiser();
private:
	float* imageInput;
	float* albedoInput;
	float* normalInput;
	float* imageOutput;
	int width;
	int height;
	int nrChannels;
	bool isHDR;
	bool isSRGB;
};