#pragma once
#include "util/math/vec3.hpp"
#include "objects/hitables/hitable_list.hpp"
#include "materials/material.hpp"
#include "util/pdf/pdf.hpp"
#include "objects/hitables/scene.hpp"

class LightIntegrator
{
public:
	LightIntegrator() {}

	vec3 trace(const ray& r, Scene* scene, hitable_list* light_shape, int depth);
private:

};