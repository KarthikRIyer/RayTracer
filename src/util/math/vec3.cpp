#include "vec3.hpp"

inline vec3 unit_vector(vec3 v) {
	return v / v.length();
}

inline int maxDimension(const vec3& v) {
	return (v.x() > v.y()) ? ((v.x() > v.z()) ? 0 : 2) : ((v.y() > v.z()) ? 1 : 2);
}

vec3 permute(const vec3& v, int x, int y, int z) {
	return vec3(v[x], v[y], v[z]);
}

inline vec3 abs(const vec3& v) {
	return vec3(abs(v.x()), abs(v.y()), abs(v.z()));
}