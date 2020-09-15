#pragma once
#include "hitable.hpp"
#include "util/math/ray.hpp"
#include "util/bounding_box/aabb.hpp"
#include "util/rng/random_number.hpp"

class bvh_node : public hitable{
public:
	bvh_node() {}
	bvh_node(hitable** l, size_t n, float time0, float time1);
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	hitable* left;
	hitable* right;
	aabb box;
};

extern int box_x_compare(const void* a, const void* b);

extern int box_y_compare(const void* a, const void* b);

extern int box_z_compare(const void* a, const void* b);