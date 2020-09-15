#pragma once

#ifndef CAMERAH
#define CAMERAH
#define _USE_MATH_DEFINES

#include<math.h>
#include "util/math/ray.hpp"
#include "util/rng/random_number.hpp"
#include "../../render_process/sampling/camera_sample.hpp"

class Camera {
public:

    Camera() {}

    Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect, float aperture,
           float focus_dist, float t0, float t1) {
        time0 = t0;
        time1 = t1;
        lens_radius = aperture / 2.0f;
        float theta = vfov * (float) M_PI / 180.0f;
        float half_height = tan(theta / 2.0f);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = glm::normalize(lookfrom - lookat);
        u = glm::normalize(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }

    ray get_ray(CameraSample cameraSample);

    glm::vec3 origin;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 u, v, w;
    float lens_radius = 0;
    float time0 = 0, time1 = 0;
};

#endif // !CAMERAH
