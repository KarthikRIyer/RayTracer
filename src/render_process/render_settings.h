#pragma once
#include <string>

#include "objects/hitables/hitable.h"
#include "util/image/image.h"
#include "tile_pool.h"
struct RenderSettings {
	Image* image;
	TilePool* tilePool;
	int nSamples;
	bool DENOISE_IMAGE = false;
	std::string filePath;
};