#pragma once
#include "random_sampler.h"

class StratifiedSampler : public RandomSampler
{
public:
	StratifiedSampler(uint64_t xSamples,
		uint64_t ySamples, int sampleDimensions, bool jitterSamples, uint64_t seed);

private:
	uint64_t xSamples, ySamples;
	bool jitterSamples;
	virtual void generateSamples();
};

extern void Stratify1D(float* samples,
	int nSamples, RNG& rng, bool jitter);

extern void Stratify2D(Point2f* samples,
	int nSamplesX, int nSamplesY, RNG& rng, bool jitter);

template <typename T>
extern inline void Shuffle(T* samples,
	unsigned int count,	unsigned int numberOfDimensions, RNG& rng);

extern uint64_t round1DStratifiedSampleCount(uint64_t sampleCount);

extern StratifiedSampler createStratifiedSampler(uint64_t sampleCount,
	int sampleDimensions,
	bool jitterSamples = true,
	uint64_t seed = 12u);