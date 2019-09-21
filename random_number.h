#pragma once
#include "vec3.h"

float random_number() {
	return float(rand()) / float(RAND_MAX);
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do
	{
		p = 2.0 * vec3(random_number(), random_number(), random_number()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 random_in_unit_disk() {
	vec3 p;
	do
	{
		p = 2.0 * vec3(random_number(), random_number(), 0) - vec3(1, 1, 0);
	} while (dot(p,p) >= 1.0);
	return p;
}