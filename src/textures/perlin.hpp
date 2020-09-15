#pragma once

#include "util/math/vec3.hpp"
#include "util/rng/random_number.hpp"
#include <glm/glm.hpp>

extern inline float perlin_interp(glm::vec3 c[2][2][2], float u, float v, float w);

class perlin {
public:
    float noise(const glm::vec3 &p) const;

    float turb(const glm::vec3 &p, int depth = 7) const;

    static glm::vec3 *ranvec;
    static int *perm_x;
    static int *perm_y;
    static int *perm_z;
};

extern void permute(int *p, int n);

static glm::vec3 *perlin_generate() {
    glm::vec3 *p = new glm::vec3[256];
    for (int i = 0; i < 256; ++i) {
        p[i] = glm::normalize(glm::vec3(-1 + 2 * random_number(), -1 + 2 * random_number(), -1 + 2 * random_number()));
    }
    return p;
}

static int *perlin_generate_perm() {
    int *p = new int[256];
    for (int i = 0; i < 256; i++) {
        p[i] = i;
    }
    permute(p, 256);
    return p;
}