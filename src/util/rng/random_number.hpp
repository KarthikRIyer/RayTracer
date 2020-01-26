#pragma once
#define _USE_MATH_DEFINES

#include "util/math/vec3.hpp"

#include <random>
#include <pcg/pcg32.h>
#include <chrono>
#include <iostream>

static const float OneMinusEpsilon = 0x1.fffffep-1;

class RNG
{
public:
	RNG() {
		rng.seed(12u);
	}
	RNG(uint64_t seed);
	float randomFloat();
	double random_double();
	uint32_t randomUInt(uint32_t bound);
	void seed(uint64_t seed);
private:
	pcg32 rng;
};

extern RNG random_generator;

extern float random_number();

extern vec3 random_in_unit_sphere();

extern inline vec3 random_to_sphere(float radius, float distance_squared);

extern vec3 random_in_unit_disk();

extern vec3 random_cosine_direction();