#include "stratified_sampler.hpp"

StratifiedSampler::StratifiedSampler(uint64_t xSamples,
                                     uint64_t ySamples,
                                     unsigned int sampleDimensions,
                                     bool jitterSamples = true,
                                     uint64_t seed = 12u) : xSamples(xSamples), ySamples(ySamples),
                                                            jitterSamples(jitterSamples),
                                                            RandomSampler(xSamples * ySamples, sampleDimensions, seed) {
}

void StratifiedSampler::generateSamples() {
    for (unsigned int i = 0; i < sampleArray1D.size(); i++) {
        Stratify1D(&sampleArray1D[i][0], xSamples * ySamples, rng, jitterSamples);
        Shuffle(&sampleArray1D[i][0], xSamples * ySamples, 1, rng);
    }
    for (unsigned int i = 0; i < sampleArray2D.size(); i++) {
        Stratify2D(&sampleArray2D[i][0], xSamples, ySamples, rng, jitterSamples);
        Shuffle(&sampleArray2D[i][0], xSamples * ySamples, 1, rng);
    }
}

void Stratify1D(float *samples, uint64_t nSamples, RNG &rng, bool jitter) {
    float invNSamples = 1.0f / float(nSamples);
    for (uint64_t i = 0; i < nSamples; i++) {
        float delta = jitter ? rng.randomFloat() : 0.5f;
        samples[i] = std::min((i + delta) * invNSamples, OneMinusEpsilon);
    }
}

void Stratify2D(Point2f *samples,
                uint64_t nSamplesX, uint64_t nSamplesY, RNG &rng, bool jitter) {
    float dx = 1.0f / float(nSamplesX);
    float dy = 1.0f / float(nSamplesY);
    for (uint64_t y = 0; y < nSamplesY; y++) {
        for (uint64_t x = 0; x < nSamplesX; x++) {
            float jx = jitter ? rng.randomFloat() : 0.5f;
            float jy = jitter ? rng.randomFloat() : 0.5f;
            samples->x = std::min((x + jx) * dx, OneMinusEpsilon);
            samples->y = std::min((y + jy) * dy, OneMinusEpsilon);
            ++samples;
        }
    }
}

template<typename T>
inline void Shuffle(T *samples,
                    uint64_t count, unsigned int numberOfDimensions, RNG &rng) {
    for (uint64_t i = 0; i < count; i++) {
        uint64_t otherSampleIndex = i + rng.randomUInt(count - i);
        for (unsigned int j = 0; j < numberOfDimensions; j++) {
            std::swap(samples[numberOfDimensions * i + j],
                      samples[numberOfDimensions * otherSampleIndex + j]);
        }
    }
}

uint64_t round1DStratifiedSampleCount(uint64_t sampleCount) {
    uint64_t nextCount = (uint64_t) floor(sqrt(sampleCount)) + 1u;
    return nextCount;
}

StratifiedSampler createStratifiedSampler(uint64_t sampleCount,
                                          unsigned int sampleDimensions, bool jitterSamples, uint64_t seed) {
    uint64_t sample1DCount = round1DStratifiedSampleCount(sampleCount);
    return StratifiedSampler(sample1DCount, sample1DCount, sampleDimensions, jitterSamples, seed);
}