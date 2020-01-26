#pragma once
#include <thread>

#include "render_settings.hpp"
#include "objects/hitables/hitable.hpp"
#include "objects/hitables/scene.hpp"
#include "lighting_integrator.hpp"
#include "util/utility_functions.hpp"
#include "sampling/random_sampler.hpp"
#include "sampling/stratified_sampler.hpp"

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