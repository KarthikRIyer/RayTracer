#pragma once
#ifndef TRIANGLEH
#define TRIANGLEH
#define _USE_MATH_DEFINES

#include "hitable.h"
#include "../../util/math/vertex.h"
#include "../../util/rng/random_number.h"
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
	virtual vec3 random(const vec3& o) const;
	
	material* mat_ptr;
	vertex v1, v2, v3;
};

vec3 triangle::random(const vec3& o) const {
	float r1 = random_number();
	float r2 = random_number();
	vec3 random_point = ((1.0f - sqrt(r1)) * v1.position) + ((sqrt(r1) * (1.0f - r2)) * v2.position) + ((sqrt(r1) * (r2)) * v3.position);
	return random_point - o;
}

bool triangle::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {

	vec3 p0t = v1.position - r.origin();
	vec3 p1t = v2.position - r.origin();
	vec3 p2t = v3.position - r.origin();

	int kz = maxDimension(abs(r.direction()));
	int kx = kz + 1;
	if (kx == 3) kx = 0;
	int ky = kx + 1;
	if (ky == 3) ky = 0;
	vec3 d = permute(r.direction(), kx, ky, kz);
	p0t = permute(p0t, kx, ky, kz);
	p1t = permute(p1t, kx, ky, kz);
	p2t = permute(p2t, kx, ky, kz);

	float Sx = -d.x() / d.z();
	float Sy = -d.y() / d.z();
	float Sz = 1.0f / d.z();
	p0t.e[0] += Sx * p0t.z();
	p0t.e[1] += Sy * p0t.z();
	p1t.e[0] += Sx * p1t.z();
	p1t.e[1] += Sy * p1t.z();
	p2t.e[0] += Sx * p2t.z();
	p2t.e[1] += Sy * p2t.z();

	float e0 = p1t.x() * p2t.y() - p1t.y() * p2t.x();
	float e1 = p2t.x() * p0t.y() - p2t.y() * p0t.x();
	float e2 = p0t.x() * p1t.y() - p0t.y() * p1t.x();

	if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0)) return false;
	float det = e0 + e1 + e2;
	if (det == 0) return false;

	p0t.e[2] *= Sz;
	p1t.e[2] *= Sz;
	p2t.e[2] *= Sz;
	float tScaled = e0 * p0t.z() + e1 * p1t.z() + e2 * p2t.z();
	if (det < 0 && (tScaled >= 0 || tScaled < tmax * det)) return false;
	else if (det > 0 && (tScaled <= 0 || tScaled > tmax* det)) return false;

	float invDet = 1.0f / det;
	float b0 = e0 * invDet;
	float b1 = e1 * invDet;
	float b2 = e2 * invDet;
	float t = tScaled * invDet;

	rec.p = b0 * v1.position + b1 * v2.position + b2 * v3.position;
	rec.t = t;
	rec.u = b0 * v1.u + b1 * v2.u + b2 * v2.u;
	rec.v = b0 * v1.v+ b1 * v2.v + b2 * v2.v;
	rec.normal = b0 * v1.normal + b1 * v2.normal + b2 * v3.normal;
	rec.mat_ptr = mat_ptr;
	return true;
}

bool triangle::bounding_box(float t0, float t1, aabb& box) const {
	float minX = std::min(v1.position.x(), std::min(v2.position.x(), v3.position.x()));
	float minY = std::min(v1.position.y(), std::min(v2.position.y(), v3.position.y()));
	float minZ = std::min(v1.position.z(), std::min(v2.position.z(), v3.position.z()));
	float maxX = std::max(v1.position.x(), std::max(v2.position.x(), v3.position.x()));
	float maxY = std::max(v1.position.y(), std::max(v2.position.y(), v3.position.y()));
	float maxZ = std::max(v1.position.z(), std::max(v2.position.z(), v3.position.z()));
	box = aabb(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));
	return true;
}

#endif // !TRIANGLEH
