#pragma once
#ifndef SPHEREH
#define SPHEREH
#define _USE_MATH_DEFINES

#include "hitable.hpp"
#include "../../util/math/onb.hpp"
#include "../../util/rng/random_number.hpp"
#include <math.h>

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	virtual float pdf_value(const vec3& o, const vec3& v)const;
	virtual vec3 random(const vec3& o) const;
	vec3 center;
	float radius;
	material *mat_ptr;
};

class moving_sphere : public hitable {
public:
	moving_sphere() {}
	moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	vec3 center(float time)const;
	vec3 center0, center1;
	float time0, time1;
	float radius;
	material* mat_ptr;
};

#endif // !SPHEREH
