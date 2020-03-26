#pragma once
#ifndef HITABLEH
#define HITABLEH
#define _USE_MATH_DEFINES

#include <math.h>
#include <float.h>
#include "util/math/ray.hpp"
#include "util/bounding_box/aabb.hpp"

class material;

extern void get_sphere_uv(const vec3& p, float& u, float& v);

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
	virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
	virtual float pdf_value(const vec3& o, const vec3& v)const { return 0.0f; }
	virtual vec3 random(const vec3& o) const { return vec3(1.0f, 0.0f, 0.0f); }
};

class flip_normals : public hitable {
public:
	flip_normals(hitable* p) : ptr(p) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
		if (ptr->hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else {
			return false;
		}
	}

	virtual float pdf_value(const vec3& o, const vec3& v)const {
		return ptr->pdf_value(o, v);
	}

	virtual vec3 random(const vec3& o) const {
		return ptr->random(o);
	}

	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		return ptr->bounding_box(t0, t1, box);
	}
	hitable* ptr;
};

class translate : public hitable {
public:
	translate(hitable* p, const vec3& displacement) : ptr(p), offset(displacement) {}
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	virtual float pdf_value(const vec3& o, const vec3& v)const {
		return ptr->pdf_value(o, v);
	}

	virtual vec3 random(const vec3& o) const {
		return ptr->random(o);
	}
	hitable *ptr;
	vec3 offset;
};

///////////////////////////////////////////////////////////
class rotate_y : public hitable {
public:
	rotate_y(hitable* p, float angle);
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = bbox;
		return hasbox;
	}
	virtual float pdf_value(const vec3& o, const vec3& v)const {
		return ptr->pdf_value(o, v);
	}

	virtual vec3 random(const vec3& o) const {
		return ptr->random(o);
	}
	hitable* ptr;
	float sin_theta;
	float cos_theta;
	bool hasbox;
	aabb bbox;
};

///////////////////////////////////////////////////////

class rotate_x : public hitable {
public:
	rotate_x(hitable* p, float angle);
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = bbox;
		return hasbox;
	}
	virtual float pdf_value(const vec3& o, const vec3& v)const {
		return ptr->pdf_value(o, v);
	}

	virtual vec3 random(const vec3& o) const {
		return ptr->random(o);
	}
	hitable* ptr;
	float sin_theta;
	float cos_theta;
	bool hasbox;
	aabb bbox;
};

//////////////////////////////////////////////////////////////////

class rotate_z : public hitable {
public:
	rotate_z(hitable* p, float angle);
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = bbox;
		return hasbox;
	}
	virtual float pdf_value(const vec3& o, const vec3& v)const {
		return ptr->pdf_value(o, v);
	}

	virtual vec3 random(const vec3& o) const {
		return ptr->random(o);
	}
	hitable* ptr;
	float sin_theta;
	float cos_theta;
	bool hasbox;
	aabb bbox;
};

//////////////////////////////////////////////////////////////////

class scale : public hitable {
public:
	scale(hitable* p, float s);
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = bbox;
		return hasbox;
	}
	virtual float pdf_value(const vec3& o, const vec3& v)const {
		return ptr->pdf_value(o, v);
	}

	virtual vec3 random(const vec3& o) const {
		return ptr->random(o);
	}
	hitable* ptr;
	float m_scale;
	float m_inv_scale;
	bool hasbox;
	aabb bbox;
};

#endif // !HITABLEH
