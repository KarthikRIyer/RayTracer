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

	void executeDenoiser() {
		oidn::Format format;
		switch (nrChannels)
		{
			case 1:
				format = oidn::Format::Float;
				break;
			case 2:
				format = oidn::Format::Float2;
				break;
			case 3:
				format = oidn::Format::Float3;
				break;
			case 4:
				format = oidn::Format::Float4;
				break;
			default:
				format = oidn::Format::Float3;
				break;
		}

		oidn::DeviceRef device = oidn::newDevice();
		device.commit();
		oidn::FilterRef filter = device.newFilter("RT");

		filter.setImage("color", imageInput, format, width, height);
		filter.setImage("output", imageOutput, format, width, height);
		if (albedoInput != NULL) {
			filter.setImage("albedo", albedoInput, format, width, height);
		}
		if (normalInput != NULL) {
			filter.setImage("normal", normalInput, format, width, height);
		}
		filter.set("hdr", isHDR);
		filter.set("srgb", isSRGB);
		filter.commit();

		filter.execute();

		const char* errorMessage;
		if (device.getError(errorMessage) != oidn::Error::None) {
			std::cout << "OIDN::Error:: " << errorMessage << "\n";
		}
	}
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