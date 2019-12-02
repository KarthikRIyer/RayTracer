#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include<iostream>
#include<fstream>
#include<algorithm>
#include<chrono>
#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "random_number.h"
#include "sphere.h"
#include "rect.h"
#include "box.h"
#include "constant_medium.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "bvh_node.h"
#include "pdf.h"
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

int GRADIENT_SKY = 0;
int BLACK_SKY = 1;

int SKY = BLACK_SKY;

int nx = 1080 / 5;
int ny = 1080 / 5;
int ns = 100;

vec3 lookfrom(0,0,0);
vec3 lookat(0,0,0);
float dist_to_focus = 0;
float aperture = 0.0;
float vfov = 40.0;
camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
hitable* light_hitable;

vec3 color(const ray& r, hitable* world, hitable* light_shape, int depth) {
	hit_record hrec;
	if (world->hit(r, 0.001, FLT_MAX, hrec)) {
		scatter_record srec;
		vec3 emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
		if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec)) {
			if (srec.is_specular) {
				return srec.attenuation * color(srec.specular_ray, world, light_shape, depth+1);
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

//hitable* random_scene() {
//	lookfrom = vec3(13, 2, 3);
//	lookat = vec3(0, 0, 0);
//	dist_to_focus = 10.0;
//	aperture = 0.1;
//	vfov = 20.0;
//	cam = camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
//
//	SKY = GRADIENT_SKY;
//	int n = 500;
//	hitable** list = new hitable * [n + 1];
//
//	texture* checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
//
//	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
//
//	int i = 1;
//	for (int a = -11; a < 11; a++) {
//		for (int b = -11; b < 11; b++)
//		{
//			float choose_mat = random_number();
//			vec3 center(a + 0.9 * random_number(), 0.2, b + 0.9 * random_number());
//			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
//				if (choose_mat < 0.8)
//				{
//					list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * random_number(), 0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(random_number() * random_number(), random_number() * random_number(), random_number() * random_number()))));
//				}
//				else if (choose_mat < 0.95)
//				{
//					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1.0 + random_number()), 0.5 * (1.0 + random_number()), 0.5 * (1.0 + random_number())), 0.5 * (random_number())));
//				}
//				else
//				{
//					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
//				}
//			}
//		}
//	}
//	int nx, ny, nn;
//	unsigned char* tex_data = stbi_load("earth-map.jpg", &nx, &ny, &nn, 0);
//	
//	material* mat = new lambertian(new image_texture(tex_data, nx, ny));
//
//	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
//	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
//	list[i++] = new sphere(vec3(4, 1, 0), 1.0, mat);
//	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
//
//	hitable** f_list = new hitable * [1];
//	f_list[0] = new bvh_node(list, i, 0, 1);
//
//	return new hitable_list(f_list, 1);
//
//}

//hitable* simple_light() {
//	lookfrom = vec3(13, 2, 3);
//	lookat = vec3(0, 0, 0);
//	dist_to_focus = 10.0;
//	aperture = 0.1;
//	vfov = 40.0;
//	cam = camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
//
//	SKY = GRADIENT_SKY;
//	hitable** list = new hitable * [4];
//	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
//	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
//	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
//	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
//
//	light_hitable = list[3];
//
//	return new hitable_list(list, 4);
//}

//hitable* two_perlin_spheres() {
//	lookfrom = vec3(13, 2, 3);
//	lookat = vec3(0, 0, 0);
//	dist_to_focus = 10.0;
//	aperture = 0.0;
//	vfov = 20.0;
//	cam = camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
//
//	SKY = GRADIENT_SKY;
//	texture* pertext = new noise_texture(4);
//	hitable** list = new hitable * [2];
//	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
//	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
//	return new hitable_list(list, 2);
//}

hitable* cornell_box() {
	SKY = BLACK_SKY;

	lookfrom = vec3(278, 278, -800);
	lookat = vec3(278, 278, 0);
	dist_to_focus = 10.0;
	aperture = 0.0;
	vfov = 40.0;
	cam = camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	hitable** list = new hitable * [8];
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
	//list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new sphere(vec3(190.0f, 90.0f, 190.0f), 90, glass);
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));

	return new hitable_list(list, i);
}

//hitable* cornell_smoke() {
//
//	lookfrom = vec3(278, 278, -800);
//	lookat = vec3(278, 278, 0);
//	dist_to_focus = 10.0;
//	aperture = 0.0;
//	vfov = 40.0;
//	cam = camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
//
//	SKY = BLACK_SKY;
//	hitable** list = new hitable * [8];
//	int i = 0;
//	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
//	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
//	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
//	material* light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
//	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
//	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
//	list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
//	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
//	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
//	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
//	hitable* b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
//	hitable* b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
//	list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
//	list[i++] = new constant_medium(b2, 0.01, new constant_texture(vec3(0.0, 0.0, 0.0)));
//
//	light_hitable = list[2];
//
//	return new hitable_list(list, i);
//}

//hitable* final_scene() {
//
//	lookfrom = vec3(278, 278, -800);
//	lookat = vec3(278, 278, 0);
//	dist_to_focus = 10.0;
//	aperture = 0.0;
//	vfov = 40.0;
//	cam = camera(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);
//
//	auto start = std::chrono::high_resolution_clock::now();
//	cout << "Building BVH\n";
//	int nb = 20;
//	hitable** list = new hitable * [30];
//	hitable** box_list = new hitable * [10000];
//	hitable** box_list2 = new hitable * [10000];
//
//	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
//	material* ground = new lambertian(new constant_texture(vec3(0.48, 0.83, 0.53)));
//	int b = 0;
//	for (int i = 0; i < nb; i++) {
//		for (int j = 0; j < nb; j++) {
//			float w = 100;
//			float x0 = -1000 + i * w;
//			float z0 = -1000 + j * w;
//			float y0 = 0;
//			float x1 = x0 + w;
//			float y1 = 100 * (random_number() + 0.01);
//			float z1 = z0 + w;
//			box_list[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
//		}
//	}
//	int l = 0;
//	list[l++] = new bvh_node(box_list, b, 0, 1);
//	material* light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
//	light_hitable = list[l++] = new xz_rect(123, 423, 147, 412, 554, light);
//	vec3 center(400, 400, 200);
//	list[l++] = new moving_sphere(center, center + vec3(30, 0, 0), 0, 1, 50, new lambertian(new constant_texture(vec3(0.7, 0.3, 0.1))));
//	list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
//	list[l++] = new sphere(vec3(0, 150, 45), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));
//
//	hitable* boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
//	list[l++] = boundary;
//	list[l++] = new constant_medium(boundary, 0.2, new constant_texture(vec3(0.2, 0.4, 0.9)));
//
//	boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
//	list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(vec3(1.0, 1.0, 1.0)));
//	int nx, ny, nn;
//	unsigned char* tex_data = stbi_load("earth-map.jpg", &nx, &ny, &nn, 0);
//	material* emat = new lambertian(new image_texture(tex_data, nx, ny));
//	list[l++] = new sphere(vec3(400, 200, 400), 100, emat);
//	texture* pertext = new noise_texture(0.1);
//	list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(pertext));
//	int ns = 1000;
//	for (int j = 0; j < ns; j++) {
//		box_list2[j] = new sphere(vec3(165 * random_number(), 165 * random_number(), 165 * random_number()), 10, white);
//	}
//	list[l++] = new translate(new rotate_y(new bvh_node(box_list2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));
//	//return new hitable_list(list, l);
//
//	hitable** f_list = new hitable * [1];	
//	f_list[0] = new bvh_node(list, l, 0, 1);
//	cout << "Built BVH\n";
//	auto end = std::chrono::high_resolution_clock::now();
//	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//	cout << "BVH Building time = " << duration.count() << "\n";
//	return new hitable_list(f_list, 1);
//}

int clamp(int a, int min, int max) {
	return std::max(min, std::min(a, max));
}

inline vec3 de_nan(const vec3& c) {
	vec3 temp = c;
	if (!(temp[0] == temp[0])) temp[0] = 0;
	if (!(temp[1] == temp[1])) temp[1] = 0;
	if (!(temp[2] == temp[2])) temp[2] = 0;
	return temp;
}

int main() {

	/*ofstream fout;
	fout.open("img.ppm");*/

	/*hitable* list[5];

	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));*/

	//hitable* world = new hitable_list(list, 5);

	//scene setup for random scene
	/*hitable* world = random_scene();
	*/

	//scene setup for simple light
	/*hitable* world = simple_light();
	*/

	//scene setup for cornell box
	hitable* world = cornell_box();
	auto start = std::chrono::high_resolution_clock::now();
	
	light_hitable = new xz_rect(213, 343, 227, 332, 554, 0);
	hitable* glass_sphere = new sphere(vec3(190.0f, 90.0f, 190.0f), 90, 0);
	hitable* a[2];
	a[0] = light_hitable;
	a[1] = glass_sphere;
	hitable_list hlist(a,2);
	//scene setup for cornell smoke
	/*hitable* world = cornell_smoke();
	;*/

	//scene setup for final scene
	/*hitable* world = final_scene();
	auto start = std::chrono::high_resolution_clock::now();
	*/

	//scene setup for perlin sphere
	/*hitable* world = two_perlin_spheres();
	*/

	unsigned char* buffer = new unsigned char[nx * ny * 3];
	for (int j = ny - 1; j >= 0; j--) {

		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = (float(i) + random_number()) / float(nx);
				float v = (float(j) + random_number()) / float(ny);

				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += de_nan(color(r, world, &hlist, 0));
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			buffer[3 * i + 3 * nx * (ny - 1 - j)] = clamp(ir, 0, 255);
			buffer[3 * i + 3 * nx * (ny - 1 - j) + 1] = clamp(ig, 0, 255);
			buffer[3 * i + 3 * nx * (ny - 1 - j) + 2] = clamp(ib, 0, 255);
		}

	}

	stbi_write_png("img.png", nx, ny, 3, buffer, nx * 3);

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
	cout << "Rendering time = " << duration.count() << "\n";

}