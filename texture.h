#pragma once
#include "vec3.h"
#include "perlin.h"

class texture {
public:
	virtual vec3 value(float u, float v, const vec3& p)const = 0;
};

class constant_texture : public texture {
public:
	constant_texture() {}
	constant_texture(vec3 c) : color(c) {}
	virtual vec3 value(float u, float v, const vec3& p) const {
		return color;
	}
	vec3 color;
};

class checker_texture : public texture {
public:
	checker_texture() {}
	checker_texture(texture *t0, texture *t1) : even(t0), odd(t1) {}
	virtual vec3 value(float u, float v, const vec3& p) const {
		float sines = sin(10.0 * p.x()) * sin(10.0 * p.y()) * sin(10.0 * p.z());
		if (sines < 0.0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
	texture* even;
	texture* odd;
};

class image_texture : public texture {
public:
	image_texture() {}
	image_texture(unsigned char *pixels, int A, int B) : data(pixels), nX(A), nY(B) {}
	virtual vec3 value(float u, float v, const vec3& p) const;
	unsigned char* data;
	int nX, nY;
};

vec3 image_texture::value(float u, float v, const vec3& p) const {
	int i = u * nX;
	int j = (1.0 - v) * nY - 0.001;
	if (i < 0)i = 0;
	if (j < 0)j = 0;
	if (i > nX - 1)i = nX - 1;
	if (j > nY - 1)j = nY - 1;
	float r = int(data[3 * i + 3 * nX * j]) / 255.0;
	float g = int(data[3 * i + 3 * nX * j + 1]) / 255.0;
	float b = int(data[3 * i + 3 * nX * j + 2]) / 255.0;
	return vec3(r, g, b);
}

class noise_texture : public texture {
public:
	noise_texture() {}
	noise_texture(float sc) : scale(sc) {}
	virtual vec3 value(float u, float v, const vec3& p) const {
		return vec3(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
	}
	perlin noise;
	float scale;
};