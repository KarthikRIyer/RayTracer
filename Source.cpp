#define STB_IMAGE_IMPLEMENTATION

#include<iostream>
#include<fstream>
#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "random_number.h"
#include "sphere.h"
#include "rect.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "stb_image.h"

using namespace std;

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
		/*vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);*/
		return vec3(0, 0, 0);
	}
}

hitable* random_scene() {

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

hitable* simple_light() {
	hitable** list = new hitable * [4];
	list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
	return new hitable_list(list, 4);
}

int main() {
	ofstream fout;
	fout.open("img.ppm");
	int nx = 1920/5;
	int ny = 1080/5;
	int ns = 128;

	/*hitable* list[5];
	
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));*/

	//hitable* world = new hitable_list(list, 5);
	hitable* world = simple_light();
	vec3 lookfrom(13,2,3);
	vec3 lookat(0,0,0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 40, float(nx) / float(ny), aperture, dist_to_focus, 0.0, 1.0);

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