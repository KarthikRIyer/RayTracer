#pragma once
#include "util/math/vec3.h"
#include "objects/hitables/hitable_list.h"
#include "materials/material.h"
#include "util/pdf/pdf.h"
#include "objects/hitables/scene.h"

class LightIntegrator
{
public:
	LightIntegrator() {}

	vec3 trace(const ray& r, Scene* scene, hitable_list* light_shape, int depth);
private:

};