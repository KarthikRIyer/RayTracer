#include "texture.h"
#include "util/image/image.h"

image_texture::image_texture(std::string path) {
	image = Image(path);
	data = image.getBuffer();
}

vec3 image_texture::value(float u, float v, const vec3& p) const {
	int i = u * nX;
	int j = (1.0 - v) * nY - 0.001;
	if (i < 0)i = 0;
	if (j < 0)j = 0;
	if (i > nX - 1)i = nX - 1;
	if (j > nY - 1)j = nY - 1;
	float r = data[3 * i + 3 * nX * j];
	float g = data[3 * i + 3 * nX * j + 1];
	float b = data[3 * i + 3 * nX * j + 2];
	return vec3(r, g, b);
}