#pragma once
#ifndef VERTEXH
#define VERTEXH

#include "vec3.hpp"
#include <glm/glm.hpp>

class vertex {
public:
	vertex() {}
	vertex(glm::vec3 p, glm::vec3 n, float ut, float vt): position(p), normal(n), u(ut), v(vt) {}

	glm::vec3 position;
	glm::vec3 normal;
	float u = 0, v = 0;

};

#endif