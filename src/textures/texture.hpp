#pragma once
#include<string>
#include<iostream>

#include "util/math/vec3.hpp"
#include "util/image/image.hpp"
#include "perlin.hpp"

class texture {
public:
	virtual glm::vec3 value(float u, float v, const glm::vec3& p)const = 0;
};

class constant_texture : public texture {
public:
	constant_texture() {}
	constant_texture(glm::vec3 c) : color(c) {}
	virtual glm::vec3 value(float u, float v, const glm::vec3& p) const {
		return color;
	}
	glm::vec3 color;
};

class checker_texture : public texture {
public:
	checker_texture() {}
	checker_texture(texture *t0, texture *t1) : even(t0), odd(t1) {}
	virtual glm::vec3 value(float u, float v, const glm::vec3& p) const {
		float sines = sin(10.0f * p.r) * sin(10.0f * p.g) * sin(10.0f * p.b);
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
	image_texture(float *pixels, int A, int B) : data(pixels), nX(A), nY(B)
	{
		image.setBuffer(pixels, A, B);
	}
	image_texture(std::string path);
	virtual glm::vec3 value(float u, float v, const glm::vec3& p) const;
	float* data;
	Image image;
	int nX, nY, nC = 3;
};

class noise_texture : public texture {
public:
	noise_texture() {}
	noise_texture(float sc) : scale(sc) {}
	virtual glm::vec3 value(float u, float v, const glm::vec3& p) const {
		return glm::vec3(1, 1, 1) * 0.5f * (1 + sin(scale * p.z + 10 * noise.turb(p)));
	}
	perlin noise;
	float scale;
};