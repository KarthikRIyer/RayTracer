#include "onb.hpp"

void onb::build_from_w(const glm::vec3 &n) {
//	axis[2] = unit_vector(n);
    axis[2] = glm::normalize(n);
    glm::vec3 a;
    if (fabs(w().x) > 0.9f) {
        a = glm::vec3(0.0f, 1.0f, 0.0f);
    } else {
        a = glm::vec3(1.0f, 0.0f, 0.0f);
    }
//    axis[1] = unit_vector(cross(w(), a));
    axis[1] = glm::normalize(glm::cross(w(), a));
    axis[0] = glm::cross(w(), v());
}