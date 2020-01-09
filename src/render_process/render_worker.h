#pragma once
#include "render_settings.h"
#include "../objects/hitables/hitable.h"
#include "../objects/hitables/scene.h"
#include "lighting_integrator.h"
#include "../util/utility_functions.h"
#include <thread>
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

void RenderWorker::execute() {

	LightIntegrator lightIntegrator;

	TilePool* tilePool = renderSettings.tilePool;
	Image* image = renderSettings.image;
	int renderSamples = renderSettings.nSamples;
	float invRenderSamples = 1.0f / (float)renderSamples;
	Camera camera = scene->camera;

	while (tilePool->getPoolSize() > 0)
	{
		Tile tile = tilePool->getNextTile();

		for (int j = tile.yMax - 1; j >= tile.yMin; j--) {

			for (int i = tile.xMin; i < tile.xMax; i++) {
				vec3 col(0, 0, 0);
				for (int s = 0; s < renderSamples; s++) {
					float u = (float(i) + random_number()) / float(image->getWidth());
					float v = (float(j) + random_number()) / float(image->getHeight());

					ray r = camera.get_ray(u, v);
					vec3 p = r.point_at_parameter(2.0);
					col += de_nan(lightIntegrator.trace(r, scene, scene->getLightHitableList(), 0));
				}
				col *= invRenderSamples;
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

				(*image)[3 * i + 3 * image->getWidth() * (image->getHeight() - 1 - j)] = col[0];
				(*image)[3 * i + 3 * image->getWidth() * (image->getHeight() - 1 - j) + 1] = col[1];
				(*image)[3 * i + 3 * image->getWidth() * (image->getHeight() - 1 - j) + 2] = col[2];

			}

		}
	}
}
