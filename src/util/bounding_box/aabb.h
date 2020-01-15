#pragma once
#include "util/math/vec3.h"
#include "util/math/ray.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb {
public:
	aabb() {}
	aabb(const vec3& a, const vec3& b) {
		_min = a;
		_max = b;
	}

	vec3 min() const { return _min; }
	vec3 max() const { return _max; }

	bool hit(const ray& r, float tmin, float tmax) const {
		for (int i = 0; i < 3; i++)
		{
			float t0 = ffmin((_min[i]-r.origin()[i])/r.direction()[i], (_max[i] - r.origin()[i]) / r.direction()[i]);
			float t1 = ffmax((_min[i] - r.origin()[i]) / r.direction()[i], (_max[i] - r.origin()[i]) / r.direction()[i]);

			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);
			if (tmax <= tmin) {
				return false;
			}
		}
		return true;
	}

	vec3 _min;
	vec3 _max;
};

extern aabb surrounding_box(aabb box0, aabb box1);