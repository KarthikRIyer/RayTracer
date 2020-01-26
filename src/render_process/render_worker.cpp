#include "render_worker.hpp"

void RenderWorker::execute() {

	LightIntegrator lightIntegrator;

	TilePool* tilePool = renderSettings.tilePool;
	Image* image = renderSettings.image;
	int renderSamples = renderSettings.nSamples;
	float invRenderSamples = 1.0f / (float)renderSamples;
	Camera camera = scene->camera;

	RandomSampler randomSampler(renderSamples, 5, rand());

	float invWidth = 1.0f / float(image->getWidth());
	float invHeight = 1.0f / float(image->getHeight());

	while (tilePool->getPoolSize() > 0)
	{
		Tile tile = tilePool->getNextTile();

		for (int j = tile.yMax - 1; j >= tile.yMin; j--) {

			for (int i = tile.xMin; i < tile.xMax; i++) {
				vec3 col(0, 0, 0);
				Point2i currentPixel(i, j);
				randomSampler.startPixel(currentPixel);
				for (int s = 0; s < renderSamples; s++) {

					randomSampler.startNextSample();
					CameraSample cameraSample = randomSampler.getCameraSample(currentPixel);
					cameraSample.pFilm.x *= invWidth;
					cameraSample.pFilm.y *= invHeight;

					ray r = camera.get_ray(cameraSample);
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
