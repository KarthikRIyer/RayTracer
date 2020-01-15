#pragma once
#include <vector>
#include <iostream>

#include "camera_sample.h"

class Sampler
{
public:
	Sampler(uint64_t samplesPerPixel) : samplesPerPixel(samplesPerPixel) {}
	virtual float get1D() = 0;
	virtual Point2f get2D() = 0;
	CameraSample getCameraSample(Point2i& currentPixel);
	virtual void startPixel(Point2i& p);
	virtual bool startNextSample();
private:
	size_t current1DOffset = 0, current2DOffset = 0;
protected:
	std::vector<std::vector<float>> sampleArray1D;
	std::vector<std::vector<Point2f>> sampleArray2D;
	Point2i currentPixel;
	uint64_t samplesPerPixel;
	uint64_t currentSampleIndex = 0;
};
