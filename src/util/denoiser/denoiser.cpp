#include "denoiser.h"

void Denoiser::executeDenoiser() {
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