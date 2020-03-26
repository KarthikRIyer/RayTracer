#include "vec3.hpp"

vec3 permute(const vec3& v, int x, int y, int z) {
	return vec3(v[x], v[y], v[z]);
}
