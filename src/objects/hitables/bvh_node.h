#pragma once
#include "hitable.h"
#include "util/math/ray.h"
#include "util/bounding_box/aabb.h"
#include "util/rng/random_number.h"

class bvh_node : public hitable{
public:
	bvh_node() {}
	bvh_node(hitable** l, int n, float time0, float time1);
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	hitable* left;
	hitable* right;
	aabb box;
};

extern int box_x_compare(const void* a, const void* b);

extern int box_y_compare(const void* a, const void* b);

extern int box_z_compare(const void* a, const void* b);