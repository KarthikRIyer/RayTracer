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
	sphere(glm::vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	virtual float pdf_value(const glm::vec3& o, const glm::vec3& v)const;
	virtual glm::vec3 random(const glm::vec3& o) const;
	glm::vec3 center;
	float radius;
	material *mat_ptr;
};

class moving_sphere : public hitable {
public:
	moving_sphere() {}
	moving_sphere(glm::vec3 cen0, glm::vec3 cen1, float t0, float t1, float r, material *m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	glm::vec3 center(float time)const;
	glm::vec3 center0, center1;
	float time0, time1;
	float radius;
	material* mat_ptr;
};

#endif // !SPHEREH
