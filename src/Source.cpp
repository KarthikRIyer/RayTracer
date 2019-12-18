#define STB_IMAGE_IMPLEMENTATION

#include<iostream>
#include<fstream>
#include<algorithm>
#include<chrono>
#include<thread>
#include "objects/hitables/hitable_list.h"
#include "objects/hitables/bvh_node.h"
#include "objects/hitables/sphere.h"
#include "objects/hitables/rect.h"
#include "objects/hitables/box.h"
#include "objects/hitables/constant_medium.h"
#include "objects/hitables/triangle.h"
#include "objects/hitables/model.h"
#include "objects/camera/camera.h"
#include "util/math/vec3.h"
#include "util/math/ray.h"
#include "util/pdf/pdf.h"
#include "util/rng/random_number.h"
#include "util/stb/stb_image.h"
#include "util/image/image.h"
#include "util/denoiser/denoiser.h"
#include "render_process/tile_pool.h"
#include "render_process/render_settings.h"
#include "materials/material.h"
#include "textures/texture.h"

using namespace std;

int GRADIENT_SKY = 0;
int BLACK_SKY = 1;

int SKY = BLACK_SKY;

int nx = 1080 / 2;
int ny = 1080 / 2;
int ns = 500;
bool DENOISE_IMAGE = false;
int maxThreads;

vec3 lookfrom(0,0,0);
vec3 lookat(0,0,0);
float dist_to_focus = 0;
float aperture = 0.0;
float vfov = 40.0;
camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
hitable_list light_hitable_list;
vector<hitable*> lightsVector;

vec3 color(const ray& r, hitable* world, hitable_list* light_shape, int depth) {
	hit_record hrec;
	if (world->hit(r, 0.001, FLT_MAX, hrec)) {
		scatter_record srec;
		vec3 emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
		if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec)) {
			if (srec.is_specular) {
				return srec.attenuation * color(srec.specular_ray, world, light_shape, depth+1);
			}
			else {
				if (light_shape->list_size == 0)
				{
					cosine_pdf p(hrec.normal);
					ray scattered = ray(hrec.p, p.generate(), r.time());
					float pdf_val = p.value(scattered.direction());
					delete srec.pdf_ptr;
					return emitted + srec.attenuation * hrec.mat_ptr->scattering_pdf(r, hrec, scattered) * color(scattered, world, light_shape, depth + 1) / pdf_val;
				}
				else {
					hitable_pdf plight(light_shape, hrec.p);
					mixture_pdf p(&plight, srec.pdf_ptr);
					ray scattered = ray(hrec.p, p.generate(), r.time());
					float pdf_val = p.value(scattered.direction());
					delete srec.pdf_ptr;
					return emitted + srec.attenuation * hrec.mat_ptr->scattering_pdf(r, hrec, scattered) * color(scattered, world, light_shape, depth + 1) / pdf_val;
				}
			}
		}
		else {
			return emitted;
		}
	}
	else {
		if (SKY == GRADIENT_SKY)
		{
			vec3 unit_direction = unit_vector(r.direction());
			float t = 0.5 * (unit_direction.y() + 1.0);
			return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
		}
		else if (SKY == BLACK_SKY) {
			return vec3(0, 0, 0);
		}
	}
}

hitable* cornell_box() {

	nx = 1080 / 2;
	ny = 1080 / 2;
	ns = 800;

	SKY = BLACK_SKY;

	lookfrom = vec3(278, 278, -800);
	lookat = vec3(278, 278, 0);
	dist_to_focus = 10.0;
	aperture = 0.0;
	vfov = 40.0;
	cam = camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

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
	ns = 1000;

	SKY = BLACK_SKY;

	lookfrom = vec3(0, 0.3, 4);
	lookat = vec3(0, 0, 0);
	dist_to_focus = 10;
	aperture = 0;
	vfov = 40.0;
	cam = camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* ground = new lambertian(new constant_texture(vec3(0.48, 0.83, 0.53)));
	material* glass = new dielectric(1.5f);

	hitable** list = new hitable * [3];

	int i = 0;
	list[i++] = new sphere(vec3(0, -1000, 0), 998.3f, ground);
	list[i++] = new model("models/monkey.obj", white);
	list[i++] = new sphere(vec3(0, 0, 6), 1.7, new diffuse_light(new constant_texture(vec3(6, 6, 6))));

	lightsVector.clear();
	lightsVector.push_back(list[2]);

	std::cout << "Rendering Scene\n";
	return new hitable_list(list, i);

}

inline vec3 de_nan(const vec3& c) {
	vec3 temp = c;
	if (!(temp[0] == temp[0])) temp[0] = 0;
	if (!(temp[1] == temp[1])) temp[1] = 0;
	if (!(temp[2] == temp[2])) temp[2] = 0;
	return temp;
}

void renderScene(hitable* world, Image* image, TilePool* tilePool) {

	while (tilePool->getPoolSize() > 0)
	{	
		Tile tile = tilePool->getNextTile();

		for (int j = tile.yMax - 1; j >= tile.yMin; j--) {

			for (int i = tile.xMin; i < tile.xMax; i++) {
				vec3 col(0, 0, 0);
				for (int s = 0; s < ns; s++) {
					float u = (float(i) + random_number()) / float(nx);
					float v = (float(j) + random_number()) / float(ny);

					ray r = cam.get_ray(u, v);
					vec3 p = r.point_at_parameter(2.0);
					col += de_nan(color(r, world, &light_hitable_list, 0));
				}
				col /= float(ns);
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

				(*image)[3 * i + 3 * nx * (ny - 1 - j)] = col[0];
				(*image)[3 * i + 3 * nx * (ny - 1 - j) + 1] = col[1];
				(*image)[3 * i + 3 * nx * (ny - 1 - j) + 2] = col[2];

			}

		}
	}
}

int main() {

	//scene setup
	hitable* world = model_scene();
	auto start = std::chrono::high_resolution_clock::now();
	light_hitable_list = hitable_list(&lightsVector[0], lightsVector.size());
	string filePath = "img.png";

	Image image(nx, ny);
	TilePool tilePool(nx, ny, 50);

	maxThreads = std::thread::hardware_concurrency();
	//maxThreads = 1;
	std::vector<std::thread> renderThreads;
	renderThreads.resize(maxThreads);

	//RenderSettings renderSettings{world, &image, &tilePool, ns, BLACK_SKY, false, "img.png"};

	for (unsigned int i = 0; i < maxThreads; i++)
	{
		renderThreads[i] = std::thread(&renderScene, world, &image, &tilePool);
	}
	for (unsigned int i = 0; i < maxThreads; i++)
	{
		if (renderThreads[i].joinable())
		{
			renderThreads[i].join();
		}
	}

	/*for (int j = ny - 1; j >= 0; j--) {

		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = (float(i) + random_number()) / float(nx);
				float v = (float(j) + random_number()) / float(ny);

				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += de_nan(color(r, world, &light_hitable_list, 0));
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			image[3 * i + 3 * nx * (ny - 1 - j)] = col[0];
			image[3 * i + 3 * nx * (ny - 1 - j) + 1] = col[1];
			image[3 * i + 3 * nx * (ny - 1 - j) + 2] = col[2];

		}

	}*/

	if (DENOISE_IMAGE)
	{
		Image outputImage(image.getWidth(), image.getHeight());
		Denoiser imageDenoiser(image.getBuffer(), outputImage.getBuffer(), image.getWidth(), image.getHeight(), image.getChannels());
		imageDenoiser.executeDenoiser();
		outputImage.saveImage(filePath);
	}
	else
	{
		image.saveImage(filePath);
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
	cout << "Rendering time = " << duration.count() << "\n";

}