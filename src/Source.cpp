#include<iostream>
#include<fstream>
#include<chrono>
#include<thread>
#include<nlohmann/json.hpp>
#include "objects/hitables/sphere.hpp"
#include "objects/hitables/rect.hpp"
#include "objects/hitables/box.hpp"
#include "objects/hitables/model.hpp"
#include "objects/hitables/scene.hpp"
#include "objects/camera/camera.hpp"
#include "util/image/image.hpp"
#include "util/denoiser/denoiser.hpp"
#include "util/scene_parser/scene_parser.hpp"
#include "render_process/tile_pool.hpp"
#include "render_process/render_settings.hpp"
#include "render_process/render_worker.hpp"

using json = nlohmann::json;

void initializeRender(Scene* scene, RenderSettings renderSettings) {

	RenderWorker renderWorker(scene, renderSettings, std::this_thread::get_id());
	renderWorker.execute();

}

int main() {

	//scene setup
	auto startSceneSetupTime = std::chrono::high_resolution_clock::now();
	std::cout << "Building Scene.\n";
	SceneParser sceneParser("sample_scenes/cornell_box.json");

	std::string filePath = "img.png";
	Image image(sceneParser.getImageWidth(), sceneParser.getImageHeight());
	TilePool tilePool(image.getWidth(), image.getHeight(), 50);

	RenderSettings renderSettings{
		&image,
		&tilePool,
		sceneParser.getRenderSamples(),
		sceneParser.denoiseImage(),
		filePath
	};

	auto startRenderTime = std::chrono::high_resolution_clock::now();
	auto sceneBuildDuration = std::chrono::duration_cast<std::chrono::seconds>(startRenderTime - startSceneSetupTime);
	std::cout << "Scene built: " << sceneBuildDuration.count() << "\n";
	std::cout << "Starting Render.\n";

	int maxThreads = std::thread::hardware_concurrency();
	std::cout << "Using " << maxThreads << " threads.\n";
	std::vector<std::thread> renderThreads;
	renderThreads.resize(maxThreads);

	for (unsigned int i = 0; i < maxThreads; i++)
	{
		renderThreads[i] = std::thread(&initializeRender, sceneParser.getScene(), renderSettings);
	}
	for (unsigned int i = 0; i < maxThreads; i++)
	{
		if (renderThreads[i].joinable())
		{
			renderThreads[i].join();
		}
	}


	if (renderSettings.DENOISE_IMAGE)
	{
		auto startDenoiseTime = std::chrono::high_resolution_clock::now();
		auto renderDuration = std::chrono::duration_cast<std::chrono::seconds>(startDenoiseTime - startRenderTime);
		std::cout << "Render Complete: " << renderDuration.count() << " seconds\n";
		Image outputImage(image.getWidth(), image.getHeight());
		Denoiser imageDenoiser(image.getBuffer(), outputImage.getBuffer(), image.getWidth(), image.getHeight(), image.getChannels());
		imageDenoiser.executeDenoiser();
		auto denoiseCompleteTime = std::chrono::high_resolution_clock::now();
		auto denoiseDuration = std::chrono::duration_cast<std::chrono::seconds>(denoiseCompleteTime - startDenoiseTime);
		std::cout << "Denoising Complete: " << denoiseDuration.count() << " seconds\n";
		outputImage.saveImage(renderSettings.filePath);
	}
	else
	{
		auto renderCompleteTime = std::chrono::high_resolution_clock::now();
		auto renderDuration = std::chrono::duration_cast<std::chrono::seconds>(renderCompleteTime - startRenderTime);
		std::cout << "Render Complete: " << renderDuration.count() << " seconds\n";
		image.saveImage(renderSettings.filePath);
	}

}