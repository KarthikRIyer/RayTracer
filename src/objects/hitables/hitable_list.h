#pragma once
#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"
#include "util/rng/random_number.h"
#include<iostream>
#include<mutex>

class hitable_list : public hitable {
public:
	hitable_list() {}
	hitable_list(hitable** l, int n) { list = l; list_size = n; }
	//hitable_list(std::vector<hitable*> listVector) { list = &listVector[0]; list_size = listVector.size(); }
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	virtual float pdf_value(const vec3& o, const vec3& v) const;
	virtual vec3 random(const vec3& o) const;
	hitable** list;
	int list_size = 0;
};

#endif // !HITABLELISTH
