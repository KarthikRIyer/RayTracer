#pragma once
#ifndef MESHH

#include "hitable.h"
#include "triangle.h"
#include "bvh_node.h"
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

void mesh::setupMesh() {
	list_size = indices.size() / 3;
	list = new hitable * [indices.size()/3];

	for (unsigned int i = 0, j = 0; i < indices.size(); i+=3, j++)
	{
		list[j] = new triangle(vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], mat_ptr);
	}
}

vec3 mesh::random(const vec3& o) const {
	float r = random_number();
	int index = int(r * list_size);
	//std::cout << index << " " << r << "\n";
	return list[index]->random(o);
}

bool mesh::hit(const ray& r, float tmin, float tmax, hit_record& rec)const {

	if (bvhMesh->hit(r, tmin, tmax, rec)) {
		return true;
	}
	return false;

	/*hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = tmax;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, tmin, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;*/
}

bool mesh::bounding_box(float t0, float t1, aabb& box) const {
	if (list_size < 1) return false;
	aabb temp_box;
	bool first_true = list[0]->bounding_box(t0, t1, temp_box);
	if (!first_true) {
		return false;
	}
	else {
		box = temp_box;
	}
	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->bounding_box(t0, t1, temp_box)) {
			box = surrounding_box(box, temp_box);
		}
		else {
			return false;
		}
	}
	return true;
}

#endif // !MESHH
