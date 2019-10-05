#define STB_IMAGE_IMPLEMENTATION

#include<iostream>
#include<fstream>
#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "random_number.h"
#include "sphere.h"
#include "rect.h"
#include "box.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "stb_image.h"

using namespace std;

int GRADIENT_SKY = 0;
int BLACK_SKY = 1;

int SKY = BLACK_SKY;

vec3 color(const ray& r, hitable* world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return emitted + attenuation * color(scattered, world, depth + 1);
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

hitable* random_scene() {
	SKY = GRADIENT_SKY;
	int n = 500;
	hitable** list = new hitable * [n+1];

	texture* checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));

	list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(checker));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = random_number();
			vec3 center(a+0.9*random_number(), 0.2, b+0.9*random_number());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8)
				{
					list[i++] = new moving_sphere(center, center+vec3(0,0.5*random_number(),0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(random_number() * random_number(), random_number() * random_number(), random_number() * random_number()))));
				}
				else if (choose_mat < 0.95)
				{
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1.0 + random_number()), 0.5 * (1.0 + random_number()), 0.5 * (1.0 + random_number())), 0.5 * (random_number())));
				}
				else
				{
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	int nx, ny, nn;
	unsigned char* tex_data = stbi_load("earth-map.jpg", &nx, &ny, &nn, 0);
	//std::cout << nx << " " << ny << " " << nn << "\n";
	material* mat = new lambertian(new image_texture(tex_data, nx, ny));
	
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, mat);
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);

}

hitable* cornell_box() {
	SKY = BLACK_SKY;
	hitable** list = new hitable * [8];
	int i = 0;
	material* red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
	material* white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
	material* green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
	material* light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	return new hitable_list(list, i);
}

hitable* simple_light() {
	SKY = GRADIENT_SKY;
	hitable** list = new hitable * [4];
	list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	return new hitable_list(list, 4);
}

hitable* two_perlin_spheres() {
	SKY = GRADIENT_SKY;
	texture* pertext = new noise_texture(4);
	hitable** list = new hitable * [2];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
	return new hitable_list(list, 2);
}

int main() {
	ofstream fout;
	fout.open("img.ppm");
	int nx = 1080/10;
	int ny = 1080/10;
	int ns = 128;

	/*hitable* list[5];
	
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));*/

	//hitable* world = new hitable_list(list, 5);
	
	//scene setup for simple light
	/*hitable* world = random_scene();
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;
	float vfov = 20.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);*/

	//scene setup for simple light
	/*hitable* world = simple_light();
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;
	float vfov = 40.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);*/

	//scene setup for cornell box
	hitable* world = cornell_box();
	vec3 lookfrom(278,278,-800);
	vec3 lookat(278,278,0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 40.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

	//scene setup for perlin sphere
	/*hitable* world = two_perlin_spheres();
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	float vfov = 20.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);*/

	if (fout) {
	
		fout << "P3\n" << nx << " " << ny << "\n255\n";
		for (int j = ny - 1; j >= 0; j--) {
		
			for (int i = 0; i < nx; i++) {
				vec3 col(0, 0, 0);
				for (int s = 0; s < ns; s++) {
					float u = (float(i) + random_number()) / float(nx);
					float v = (float(j) + random_number()) / float(ny);

					ray r = cam.get_ray(u, v);
					vec3 p = r.point_at_parameter(2.0);
					col += color(r, world, 0);
				}
				col /= float(ns);
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

				int ir = int(255.99 * col[0]);
				int ig = int(255.99 * col[1]);
				int ib = int(255.99 * col[2]);
				fout << ir << " " << ig << " " << ib << "\n";

			}

		}

	}

}