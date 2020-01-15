#pragma once
#include "hitable.h"
#include "util/bounding_box/aabb.h"
#include "util/rng/random_number.h"

class xy_rect : public hitable {
public:
	xy_rect() {}
	xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material *mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {}
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k - 0.0001));
		return true;
	}
	virtual float pdf_value(const vec3& o, const vec3& v) const {
		hit_record rec;
		if (this->hit(ray(o, v), 0.001, FLT_MAX, rec)) {
			float area = (x1 - x0) * (y1 - y0);
			float distance_squared = rec.t * rec.t * v.squared_length();
			float cosine = fabs(dot(v, rec.normal) / v.length());
			return distance_squared / (cosine * area);
		}
		else {
			return 0;
		}
	}
	virtual vec3 random(const vec3& o) const {
		vec3 random_point = vec3(x0 + random_number() * (x1 - x0), k, y0 + random_number() * (y1 - y0));
		return random_point - o;
	}
	material* mp;
	float x0, x1, y0, y1, k;
};

class xz_rect : public hitable {
public:
	xz_rect() {}
	xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material* mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {}
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = aabb(vec3(x0, k - 0.0001, z0), vec3(x1, k - 0.0001, z1));
		return true;
	}
	virtual float pdf_value(const vec3& o, const vec3& v) const {
		hit_record rec;
		if (this->hit(ray(o, v), 0.001, FLT_MAX, rec)) {
			float area = (x1 - x0) * (z1 - z0);
			float distance_squared = rec.t * rec.t * v.squared_length();
			float cosine = fabs(dot(v, rec.normal) / v.length());
			return distance_squared / (cosine * area);
		}
		else {
			return 0;
		}
	}
	virtual vec3 random(const vec3& o) const {
		vec3 random_point = vec3(x0 + random_number() * (x1 - x0), k, z0 + random_number() * (z1 - z0));
		return random_point - o;
	}
	material* mp;
	float x0, x1, z0, z1, k;
};

class yz_rect : public hitable {
public:
	yz_rect() {}
	yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material* mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {}
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = aabb(vec3(k - 0.0001, y0, z0), vec3(k - 0.0001, y1, z1));
		return true;
	}
	virtual float pdf_value(const vec3& o, const vec3& v) const {
		hit_record rec;
		if (this->hit(ray(o, v), 0.001, FLT_MAX, rec)) {
			float area = (y1 - y0) * (z1 - z0);
			float distance_squared = rec.t * rec.t * v.squared_length();
			float cosine = fabs(dot(v, rec.normal) / v.length());
			return distance_squared / (cosine * area);
		}
		else {
			return 0;
		}
	}
	virtual vec3 random(const vec3& o) const {
		vec3 random_point = vec3(y0 + random_number() * (y1 - y0), k, z0 + random_number() * (z1 - z0));
		return random_point - o;
	}
	material* mp;
	float y0, y1, z0, z1, k;
};