#pragma once

#ifndef CAMERAH
#define CAMERAH
#define _USE_MATH_DEFINES
#include<math.h>
#include "util/math/ray.h"
#include "util/rng/random_number.h"
#include "../../render_process/sampling/camera_sample.h"

class Camera {
public:

	Camera() {}
	Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1) {
		time0 = t0;
		time1 = t1;
		lens_radius = aperture / 2.0;
		float theta = vfov * M_PI / 180.0;
		float half_height = tan(theta / 2.0);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_height * focus_dist * v;
	}

	ray get_ray(CameraSample cameraSample);

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lens_radius = 0;
	float time0 = 0, time1 = 0;
};

#endif // !CAMERAH
