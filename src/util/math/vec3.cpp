#include "vec3.hpp"

glm::vec3 permute(const glm::vec3 &v, int x, int y, int z) {
    return glm::vec3(v[x], v[y], v[z]);
}
