#pragma once
#ifndef TRIANGLEH
#define TRIANGLEH
#define _USE_MATH_DEFINES

#include "hitable.hpp"
#include "util/math/vertex.hpp"
#include "util/rng/random_number.hpp"
#include <math.h>
#include <algorithm>

class triangle : public hitable {
public:
	triangle() {}
	triangle(vertex v1, vertex v2, vertex v3, material* m) : mat_ptr(m) {
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
	}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	virtual glm::vec3 random(const glm::vec3& o) const;
	
	material* mat_ptr;
	vertex v1, v2, v3;
};

#endif // !TRIANGLEH
