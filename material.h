#pragma once
#ifndef MATERIALH
#define MATERIALH

#include<math.h>

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered)const = 0;
};

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
	{
		return false;
	}
}

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1.0 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
}

#endif // !MATERIALH
