#pragma once
#include "sampler.h"
#include "../../util/rng/random_number.h"

class RandomSampler : public Sampler
{
public:
	RandomSampler(uint64_t samplesPerPixel, uint64_t sampleDimensions, uint64_t seed);
	virtual float get1D();
	virtual Point2f get2D();
	virtual void startPixel(Point2i& p);
	virtual bool startNextSample();
private:
	RNG rng;
	virtual void generateSamples();
	uint64_t sampleDimensions;
protected:
	size_t current1DDimension = 0, current2DDimension = 0;
};

RandomSampler::RandomSampler(uint64_t samplesPerPixel, uint64_t sampleDimensions, uint64_t seed = 12u) : Sampler(samplesPerPixel), sampleDimensions(sampleDimensions)
{
	rng.seed(seed);
	for (unsigned int i = 0; i < sampleDimensions; i++)
	{
		sampleArray1D.push_back(std::vector<float>(samplesPerPixel));
		sampleArray2D.push_back(std::vector<Point2f>(samplesPerPixel));
	}
	generateSamples();
}

float RandomSampler::get1D() {
	if (current1DDimension < sampleArray1D.size())
	{
		return sampleArray1D[current1DDimension++][currentSampleIndex];
	}
	else
	{
		return rng.random_float();
	}
}

Point2f RandomSampler::get2D() {
	if (current2DDimension < sampleArray2D.size())
	{
		return sampleArray2D[current2DDimension++][currentSampleIndex];
	}
	else
	{
		return Point2f(rng.random_float(), rng.random_float());
	}
}

void RandomSampler::startPixel(Point2i& p) {
	current1DDimension = current2DDimension = 0;
	Sampler::startPixel(p);
}

bool RandomSampler::startNextSample() {
	current1DDimension = current2DDimension = 0;
	return Sampler::startNextSample();
}

void RandomSampler::generateSamples() {
	for (unsigned int i = 0; i < sampleDimensions; i++)
	{
		for (unsigned int j = 0; j < samplesPerPixel; j++)
		{
			sampleArray1D[i][j] = rng.random_float();
			sampleArray2D[i][j] = Point2f(rng.random_float(), rng.random_float());
		}
	}
}