#pragma once
#include <string>

#include "../objects/hitables/hitable.h"
#include "../util/image/image.h"
#include "tile_pool.h"
struct RenderSettings {
	hitable* world;
	Image* image;
	TilePool* tilePool;
	int nSamples;
	int SKY = 1;
	bool DENOISE_IMAGE = false;
	std::string filePath = "";
};