#pragma once
#ifndef MESHH

#include "hitable.hpp"
#include "triangle.hpp"
#include "bvh_node.hpp"
#include <vector>


class mesh : public hitable {
public:
	mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, material* m) : mat_ptr(m) {
		this->vertices = vertices;
		this->indices = indices;
		setupMesh();
		bvhMesh = new bvh_node(list, list_size, 0, 1);
	}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	virtual vec3 random(const vec3& o) const;
	void setupMesh();
	hitable** list;
	hitable* bvhMesh;
	int list_size = 0;
	material* mat_ptr;
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;

};

#endif // !MESHH
