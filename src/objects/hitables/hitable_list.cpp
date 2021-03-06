#include "hitable_list.hpp"

float hitable_list::pdf_value(const glm::vec3& o, const glm::vec3& v) const {

	float weight = 1.0f / (float)list_size;
	float sum = 0.0f;
	for (int i = 0; i < list_size; i++)
	{
		sum += weight * list[i]->pdf_value(o, v);
	}
	return sum;
}

glm::vec3 hitable_list::random(const glm::vec3& o) const {
	float r = random_number();
	int index = int(r * list_size);
	//std::cout << index << " " << r << "\n";
	return list[index]->random(o);
}

bool hitable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec) const {

	hit_record temp_rec;
	bool hit_anything = false;
	float closest_so_far = tmax;
	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, tmin, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

bool hitable_list::bounding_box(float t0, float t1, aabb& box) const {
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