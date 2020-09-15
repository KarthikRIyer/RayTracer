#pragma once

#include "random_sampler.hpp"

class StratifiedSampler : public RandomSampler {
public:
    StratifiedSampler(uint64_t xSamples,
                      uint64_t ySamples, unsigned int sampleDimensions, bool jitterSamples, uint64_t seed);

private:
    uint64_t xSamples, ySamples;
    bool jitterSamples;

    virtual void generateSamples();
};

extern void Stratify1D(float *samples,
                       uint64_t nSamples, RNG &rng, bool jitter);

extern void Stratify2D(Point2f *samples,
                       uint64_t nSamplesX, uint64_t nSamplesY, RNG &rng, bool jitter);

template<typename T>
extern inline void Shuffle(T *samples,
                           uint64_t count, unsigned int numberOfDimensions, RNG &rng);

extern uint64_t round1DStratifiedSampleCount(uint64_t sampleCount);

extern StratifiedSampler createStratifiedSampler(uint64_t sampleCount,
                                                 unsigned int sampleDimensions,
                                                 bool jitterSamples = true,
                                                 uint64_t seed = 12u);