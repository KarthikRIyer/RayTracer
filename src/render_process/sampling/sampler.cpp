#include "sampler.hpp"

void Sampler::startPixel(Point2i& p) {
	currentPixel = p;
	currentSampleIndex = 0;
	current1DOffset = current2DOffset = 0;
}

bool Sampler::startNextSample() {
	current1DOffset = current2DOffset = 0;
	return ++currentSampleIndex < samplesPerPixel;
}

CameraSample Sampler::getCameraSample(Point2i& currentPixel) {
	CameraSample cameraSample;
	Point2f random2DFilm = get2D();
	cameraSample.pFilm = Point2f(float(currentPixel.x) + random2DFilm.x, float(currentPixel.y) + random2DFilm.y);
	cameraSample.time = get1D();
	cameraSample.pLens = get2D();
	return cameraSample;
}