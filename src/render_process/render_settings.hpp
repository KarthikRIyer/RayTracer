#pragma once
#include <string>

#include "objects/hitables/hitable.hpp"
#include "util/image/image.hpp"
#include "tile_pool.hpp"
struct RenderSettings {
	Image* image;
	TilePool* tilePool;
	int nSamples;
	bool DENOISE_IMAGE = false;
	std::string filePath;
};