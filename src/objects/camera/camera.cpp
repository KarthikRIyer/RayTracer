#include "camera.h"

ray Camera::get_ray(CameraSample cameraSample) {
	vec3 rd = lens_radius * random_in_unit_disk();
	vec3 offset = u * rd.x() + v * rd.y();
	float time = time0 + cameraSample.time * (time1 - time0);
	return ray(origin + offset, lower_left_corner + cameraSample.pFilm.x * horizontal + cameraSample.pFilm.y * vertical - origin - offset, time);
}