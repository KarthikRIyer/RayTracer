#pragma once
#include <thread>

#include "render_settings.h"
#include "objects/hitables/hitable.h"
#include "objects/hitables/scene.h"
#include "lighting_integrator.h"
#include "util/utility_functions.h"
#include "sampling/random_sampler.h"
#include "sampling/stratified_sampler.h"

class RenderWorker
{
public:
	RenderWorker(Scene* scene, RenderSettings settings, std::thread::id id) : renderSettings(settings), renderWorkerId(id) {
		this->scene = scene;
	}

	RenderSettings renderSettings;

	void execute();

	std::thread::id getThreadID() {
		return  renderWorkerId;
	}

private:
	std::thread::id renderWorkerId;
	Scene* scene;

};