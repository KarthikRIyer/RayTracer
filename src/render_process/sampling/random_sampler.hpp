#pragma once
#include "sampler.hpp"
#include "util/rng/random_number.hpp"

class RandomSampler : public Sampler
{
public:
	RandomSampler(uint64_t samplesPerPixel, unsigned int sampleDimensions, uint64_t seed = 12u);
	virtual float get1D();
	virtual Point2f get2D();
	virtual void startPixel(Point2i& p);
	virtual bool startNextSample();

protected:
	unsigned int sampleDimensions;
	RNG rng;
	size_t current1DDimension = 0, current2DDimension = 0;
	virtual void generateSamples();
};