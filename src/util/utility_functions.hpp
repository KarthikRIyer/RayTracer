#pragma once
#include "math/vec3.hpp"

inline glm::vec3 de_nan(const glm::vec3& c) {
	glm::vec3 temp = c;
	if (!(temp[0] == temp[0])) temp[0] = 0;
	if (!(temp[1] == temp[1])) temp[1] = 0;
	if (!(temp[2] == temp[2])) temp[2] = 0;
	return temp;
}