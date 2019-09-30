#pragma once
#ifndef HITABLEH
#define HITABLEH
#define _USE_MATH_DEFINES

#include "ray.h"
#include <math.h>

class material;

void get_sphere_uv(const vec3& p, float& u, float& v) {
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1.0 - (phi + M_PI) / (2.0 * M_PI);
	v = (theta + M_PI / 2.0) / M_PI;
	//std::cout << u << " " << v << "\n";
}

struct hit_record {
	float t;
	float u;
	float v;
	vec3 p;
	vec3 normal;
	material *mat_ptr;
};

class hitable {
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif // !HITABLEH
