#pragma once

#include "util/math/vec3.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <random>
#include <pcg/pcg32.h>
#include <chrono>
#include <iostream>

static const float OneMinusEpsilon = 0x1.fffffep-1;

class RNG {
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

extern glm::vec3 random_in_unit_sphere();

inline glm::vec3 random_to_sphere(float radius, float distance_squared) {
    float r1 = random_number();
    float r2 = random_number();
    float z = 1 + r2 * (sqrt(1.0f - radius * radius / distance_squared) - 1.0f);
    float phi = 2.0f * (float) M_PI * r1;
    float x = cos(phi) * sqrt(1.0f - z * z);
    float y = sin(phi) * sqrt(1.0f - z * z);
    return glm::vec3(x, y, z);
}

extern glm::vec3 random_in_unit_disk();

extern glm::vec3 random_cosine_direction();
