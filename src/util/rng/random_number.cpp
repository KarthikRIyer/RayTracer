#include "random_number.h"

RNG::RNG(uint64_t seed)
{
	rng.seed(seed);
}

float RNG::randomFloat() {
	return rng.nextFloat();
}

double RNG::random_double() {
	return rng.nextDouble();
}

uint32_t RNG::randomUInt(uint32_t bound) {
	return rng.nextUInt(bound);
}

void RNG::seed(uint64_t seed) {
	rng.seed(seed);
}

RNG random_generator(12u);

float random_number() {
	return random_generator.randomFloat();
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
	} while (dot(p, p) >= 1.0);
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