#pragma once
#ifndef VERTEXH
#define VERTEXH

#include "vec3.h"

class vertex {
public:
	vertex() {}
	vertex(vec3 p, vec3 n, float ut, float vt): position(p), normal(n), u(ut), v(vt) {}

	vec3 position;
	vec3 normal;
	float u = 0, v = 0;

};

#endif