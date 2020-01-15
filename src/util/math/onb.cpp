#include "onb.h"

void onb::build_from_w(const vec3& n) {
	axis[2] = unit_vector(n);
	vec3 a;
	if (fabs(w().x()) > 0.9f) {
		a = vec3(0.0f, 1.0f, 0.0f);
	}
	else {
		a = vec3(1.0f, 0.0f, 0.0f);
	}
	axis[1] = unit_vector(cross(w(), a));
	axis[0] = cross(w(), v());
}