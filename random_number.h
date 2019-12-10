#pragma once
#include "vec3.h"
#include <random>
#include <chrono>
#include <iostream>
float random_number() {
	/*std::mt19937_64 rng;
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
	rng.seed(ss);
	std::uniform_real_distribution<float> unif(0, 1);
	float r = unif(rng);
	if (r == 1.0f) return random_number();
	return r;*/

	return (float)rand() / (float)(RAND_MAX + 1);
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do
	{
		p = 2.0 * vec3(random_number(), random_number(), random_number()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

inline vec3 random_to_sphere(float radius, float distance_squared) {
	float r1 = random_number();
	float r2 = random_number();
	float z = 1 + r2 * (sqrt(1.0f - radius * radius / distance_squared) - 1.0f);
	float phi = 2.0f * M_PI * r1;
	float x = cos(phi) * sqrt(1.0f - z * z);
	float y = sin(phi) * sqrt(1.0f - z * z);
	return vec3(x, y, z);
}

vec3 random_in_unit_disk() {
	vec3 p;
	do
	{
		p = 2.0 * vec3(random_number(), random_number(), 0) - vec3(1, 1, 0);
	} while (dot(p,p) >= 1.0);
	return p;
}

vec3 random_cosine_direction() {
	float r1 = random_number();
	float r2 = random_number();
	float z = sqrt(1.0f - r2);
	float phi = 2.0f * M_PI * r1;
	float x = cos(phi) * 2.0f * sqrt(r2);
	float y = sin(phi) * 2.0f * sqrt(r2);
	return vec3(x, y, z);
}