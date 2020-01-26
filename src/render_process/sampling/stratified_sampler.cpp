#include "stratified_sampler.hpp"

StratifiedSampler::StratifiedSampler(uint64_t xSamples,
	uint64_t ySamples,
	int sampleDimensions,
	bool jitterSamples = true,
	uint64_t seed = 12u) : xSamples(xSamples), ySamples(ySamples), jitterSamples(jitterSamples), RandomSampler(xSamples* ySamples, seed)
{
}

void StratifiedSampler::generateSamples()
{
	for (unsigned int i = 0; i < sampleArray1D.size(); i++)
	{
		Stratify1D(&sampleArray1D[i][0], xSamples * ySamples, rng, jitterSamples);
		Shuffle(&sampleArray1D[i][0], xSamples * ySamples, 1, rng);
	}
	for (unsigned int i = 0; i < sampleArray2D.size(); i++)
	{
		Stratify2D(&sampleArray2D[i][0], xSamples, ySamples, rng, jitterSamples);
		Shuffle(&sampleArray2D[i][0], xSamples * ySamples, 1, rng);
	}
}

void Stratify1D(float* samples, int nSamples, RNG& rng, bool jitter)
{
	float invNSamples = 1.0f / float(nSamples);
	for (unsigned int i = 0; i < nSamples; i++)
	{
		float delta = jitter ? rng.randomFloat() : 0.5f;
		samples[i] = std::min((i + delta) * invNSamples, OneMinusEpsilon);
	}
}

void Stratify2D(Point2f* samples,
	int nSamplesX, int nSamplesY, RNG& rng, bool jitter)
{
	float dx = 1.0f / float(nSamplesX);
	float dy = 1.0f / float(nSamplesY);
	for (unsigned int y = 0; y < nSamplesY; y++)
	{
		for (unsigned int x = 0; x < nSamplesX; x++)
		{
			float jx = jitter ? rng.randomFloat() : 0.5f;
			float jy = jitter ? rng.randomFloat() : 0.5f;
			samples->x = std::min((x + jx) * dx, OneMinusEpsilon);
			samples->y = std::min((y + jy) * dy, OneMinusEpsilon);
			++samples;
		}
	}
}

template <typename T>
inline void Shuffle(T* samples,
	unsigned int count, unsigned int numberOfDimensions, RNG& rng) {
	for (unsigned int i = 0; i < count; i++)
	{
		int otherSampleIndex = i + rng.randomUInt(count - i);
		for (unsigned int j = 0; j < numberOfDimensions; j++)
		{
			std::swap(samples[numberOfDimensions * i + j],
				samples[numberOfDimensions * otherSampleIndex + j]);
		}
	}
}

uint64_t round1DStratifiedSampleCount(uint64_t sampleCount) {
	uint64_t nextCount = floor(sqrt(sampleCount)) + 1u;
	return nextCount;
}

StratifiedSampler createStratifiedSampler(uint64_t sampleCount,
	int sampleDimensions, bool jitterSamples, uint64_t seed) {
	uint64_t sample1DCount = round1DStratifiedSampleCount(sampleCount);
	return StratifiedSampler(sample1DCount, sample1DCount, sampleDimensions, jitterSamples, seed);
}