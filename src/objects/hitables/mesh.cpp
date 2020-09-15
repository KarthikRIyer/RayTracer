#include "mesh.hpp"

void mesh::setupMesh() {
	list_size = indices.size() / 3;
	list = new hitable * [indices.size() / 3];

	for (unsigned int i = 0, j = 0; i < indices.size(); i += 3, j++)
	{
		list[j] = new triangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]], mat_ptr);
	}
}

glm::vec3 mesh::random(const glm::vec3& o) const {
	float r = random_number();
	int index = int(r * list_size);
	return list[index]->random(o);
}

bool mesh::hit(const ray& r, float tmin, float tmax, hit_record& rec)const {
	if (bvhMesh->hit(r, tmin, tmax, rec)) {
		return true;
	}
	return false;
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