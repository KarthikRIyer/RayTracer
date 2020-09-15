#include "random_number.hpp"

RNG::RNG(uint64_t seed) {
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

glm::vec3 random_in_unit_sphere() {
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(random_number(), random_number(), random_number()) - glm::vec3(1, 1, 1);
    } while (glm::length2(p) >= 1.0);
    return p;
}

glm::vec3 random_in_unit_disk() {
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(random_number(), random_number(), 0) - glm::vec3(1, 1, 0);
    } while (glm::dot(p, p) >= 1.0);
    return p;
}

glm::vec3 random_cosine_direction() {
    float r1 = random_number();
    float r2 = random_number();
    float z = sqrt(1.0f - r2);
    float phi = 2.0f * (float) M_PI * r1;
    float x = cos(phi) * 2.0f * sqrt(r2);
    float y = sin(phi) * 2.0f * sqrt(r2);
    return glm::vec3(x, y, z);
}
