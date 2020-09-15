#include "random_sampler.hpp"

RandomSampler::RandomSampler(uint64_t samplesPerPixel, unsigned int sampleDimensions, uint64_t seed) : Sampler(
        samplesPerPixel), sampleDimensions(sampleDimensions) {
    rng.seed(seed);
    for (unsigned int i = 0; i < sampleDimensions; i++) {
        sampleArray1D.emplace_back(samplesPerPixel);
        sampleArray2D.emplace_back(samplesPerPixel);
    }
}

float RandomSampler::get1D() {
    if (current1DDimension < sampleArray1D.size()) {
        return sampleArray1D[current1DDimension++][currentSampleIndex];
    } else {
        return rng.randomFloat();
    }
}

Point2f RandomSampler::get2D() {
    if (current2DDimension < sampleArray2D.size()) {
        return sampleArray2D[current2DDimension++][currentSampleIndex];
    } else {
        return Point2f(rng.randomFloat(), rng.randomFloat());
    }
}

void RandomSampler::startPixel(Point2i &p) {
    current1DDimension = current2DDimension = 0;
    generateSamples();
    Sampler::startPixel(p);
}

bool RandomSampler::startNextSample() {
    current1DDimension = current2DDimension = 0;
    return Sampler::startNextSample();
}

void RandomSampler::generateSamples() {
    for (unsigned int i = 0; i < sampleDimensions; i++) {
        for (unsigned int j = 0; j < samplesPerPixel; j++) {
            sampleArray1D[i][j] = rng.randomFloat();
            sampleArray2D[i][j] = Point2f(rng.randomFloat(), rng.randomFloat());
        }
    }
}