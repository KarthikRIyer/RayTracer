#define STB_IMAGE_IMPLEMENTATION

#include<iostream>
#include<fstream>
#include<chrono>
#include<thread>
#include "objects/hitables/sphere.h"
#include "objects/hitables/rect.h"
#include "objects/hitables/box.h"
#include "objects/hitables/model.h"
#include "objects/hitables/scene.h"
#include "objects/camera/camera.h"
#include "util/image/image.h"
#include "util/denoiser/denoiser.h"
#include "render_process/tile_pool.h"
#include "render_process/render_settings.h"
#include "render_process/render_worker.h"

int SKY;

int nx = 1080 / 2;
int ny = 1080 / 2;

vec3 lookfrom(0,0,0);
vec3 lookat(0,0,0);
float dist_to_focus = 0;
float aperture = 0.0;
float vfov = 40.0;
Camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
hitable_list lightHitableList;
std::vector<hitable*> lightsVector;

hitable* cornell_box() {

	nx = 1080 / 2;
	ny = 1080 / 2;

	SKY = Scene::BLACK_SKY;

	lookfrom = vec3(278, 278, -800);
	lookat = vec3(278, 278, 0);
	dist_to_focus = 10.0;
	aperture = 0.0;
	vfov = 40.0;
	cam = Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	hitable** list = new hitable * [9];
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	material* aluminium = new metal(vec3(0.8f, 0.85f, 0.88f), 0.0f);
	material* glass = new dielectric(1.5f);
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new flip_normals(new xz_rect(213, 343, 227, 332, 554, light));
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new sphere(vec3(190.0f, 90.0f, 190.0f), 90, glass);
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));

	lightsVector.clear();
	lightsVector.push_back(list[2]);
	lightsVector.push_back(list[6]);

	return new hitable_list(list, i);
}

hitable* model_scene() {

	std::cout << "Building Scene\n";
	nx = 1080 / 2;
	ny = 1080 / 4;

	SKY = Scene::BLACK_SKY;

	lookfrom = vec3(0, 0.3, 4);
	lookat = vec3(0, 0, 0);
	dist_to_focus = 10;
	aperture = 0;
	vfov = 40.0;
	cam = Camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* ground = new lambertian(new constant_texture(vec3(0.48, 0.83, 0.53)));
	material* glass = new dielectric(1.5f);

	hitable** list = new hitable * [3];

	int i = 0;
	list[i++] = new sphere(vec3(0, -1000, 0), 998.3f, ground);
	list[i++] = new model("models/monkey_subdivision.obj", white);
	list[i++] = new sphere(vec3(0, 0, 6), 1.7, new diffuse_light(new constant_texture(vec3(6, 6, 6))));

	lightsVector.clear();
	lightsVector.push_back(list[2]);

	std::cout << "Rendering Scene\n";
	return new hitable_list(list, i);

}


void initializeRender(Scene scene, RenderSettings renderSettings) {

	RenderWorker renderWorker(scene, renderSettings, std::this_thread::get_id());
	renderWorker.execute();

}

int main() {

	//scene setup
	auto startSceneSetupTime = std::chrono::high_resolution_clock::now();
	hitable* world = cornell_box();
	lightHitableList = hitable_list(&lightsVector[0], lightsVector.size());
	Scene scene(cam, world);
	scene.SKY = SKY;
	scene.setLightHitables(&lightHitableList);
	std::string filePath = "img.png";
	Image image(nx, ny);
	TilePool tilePool(nx, ny, 50);
	int renderSamples = 100;
	bool denoiseImage = true;

	RenderSettings renderSettings{ &image, &tilePool, renderSamples, denoiseImage, filePath };
	
	auto startRenderTime = std::chrono::high_resolution_clock::now();
	auto sceneBuildDuration = std::chrono::duration_cast<std::chrono::seconds>(startRenderTime - startSceneSetupTime);
	std::cout << "Scene built: " << sceneBuildDuration.count() << "\n";
	std::cout << "Starting Render.\n";

	int maxThreads = std::thread::hardware_concurrency();
	std::vector<std::thread> renderThreads;
	renderThreads.resize(maxThreads);

	for (unsigned int i = 0; i < maxThreads; i++)
	{
		renderThreads[i] = std::thread(&initializeRender, scene, renderSettings);
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