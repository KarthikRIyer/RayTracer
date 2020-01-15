#pragma once
#include "sampler.h"
#include "util/rng/random_number.h"

class RandomSampler : public Sampler
{
public:
	RandomSampler(uint64_t samplesPerPixel, int sampleDimensions, uint64_t seed = 12u);
	virtual float get1D();
	virtual Point2f get2D();
	virtual void startPixel(Point2i& p);
	virtual bool startNextSample();

protected:
	int sampleDimensions;
	RNG rng;
	size_t current1DDimension = 0, current2DDimension = 0;
	virtual void generateSamples();
};