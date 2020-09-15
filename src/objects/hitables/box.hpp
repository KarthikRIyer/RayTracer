#pragma once
#include "hitable_list.hpp"
#include "rect.hpp"

class box : public hitable {
public:
	box() {}
	box(const glm::vec3& p0, const glm::vec3& p1, material* ptr);
	virtual bool hit(const ray& r, float t0, float t1, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const {
		box = aabb(pmin, pmax);
		return true;
	}
	glm::vec3 pmin, pmax;
	hitable* list_ptr;
};