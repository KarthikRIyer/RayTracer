#pragma once
#ifndef MATERIALH
#define MATERIALH

#include<math.h>
#include "util/rng/random_number.hpp"
#include "util/math/ray.hpp"
#include "objects/hitables/hitable_list.hpp"
#include "textures/texture.hpp"
#include "util/math/onb.hpp"
#include "util/pdf/pdf.hpp"

struct scatter_record {
    ray specular_ray;
    bool is_specular;
    glm::vec3 attenuation;
    pdf *pdf_ptr;
};

class material {
public:
    virtual bool scatter(const ray &r_in, const hit_record &hrec, scatter_record &srec) const {
        return false;
    }

    virtual float scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const {
        return 0;
    }

    virtual glm::vec3 emitted(const ray &r_in, const hit_record &rec, float u, float v, const glm::vec3 &p) const {
        return glm::vec3(0, 0, 0);
    }
};

extern bool refract(const glm::vec3 &v, const glm::vec3 &n, float ni_over_nt, glm::vec3 &refracted);

extern glm::vec3 reflect(const glm::vec3 &v, const glm::vec3 &n);

extern float schlick(float cosine, float ref_idx);

class dielectric : public material {
public:
    dielectric(float ri) : ref_idx(ri) {}

    virtual bool scatter(const ray &r_in, const hit_record &hrec, scatter_record &srec) const {
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        srec.attenuation = glm::vec3(1.0, 1.0, 1.0);
        glm::vec3 outward_normal;
        glm::vec3 reflected = reflect(r_in.direction(), hrec.normal);
        float ni_over_nt;
        glm::vec3 refracted;
        float reflect_prob;
        float cosine;
        if (dot(r_in.direction(), hrec.normal) > 0) {
            outward_normal = -hrec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), hrec.normal) / glm::length(r_in.direction());
        } else {
            outward_normal = hrec.normal;
            ni_over_nt = 1.0f / ref_idx;
            cosine = -dot(r_in.direction(), hrec.normal) / glm::length(r_in.direction());
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        } else {
            reflect_prob = 1.0;
        }
        if (random_number() < reflect_prob) {
            srec.specular_ray = ray(hrec.p, reflected);
        } else {
            srec.specular_ray = ray(hrec.p, refracted);
        }
        return true;
    }

    float ref_idx;
};

class metal : public material {
public:
    metal(const glm::vec3 &a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }

    virtual bool scatter(const ray &r_in, const hit_record &hrec, scatter_record &srec) const {
        glm::vec3 reflected = reflect(glm::normalize(r_in.direction()), hrec.normal);
        srec.specular_ray = ray(hrec.p, reflected + fuzz * random_in_unit_sphere());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        return true;
    }

    glm::vec3 albedo;
    float fuzz;
};

class lambertian : public material {
public:
    lambertian(texture *a) : albedo(a) {}

    float scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const {
        float cosine = dot(rec.normal, glm::normalize(scattered.direction()));
        if (cosine < 0) {
            cosine = 0;
        }
        return cosine / (float) M_PI;
    }

    virtual bool scatter(const ray &r_in, const hit_record &hrec, scatter_record &srec) const {
        srec.is_specular = false;
        srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.p);
        srec.pdf_ptr = new cosine_pdf(hrec.normal);
        return true;
    }

    texture *albedo;
};

class diffuse_light : public material {
public:
    diffuse_light(texture *a) : emit(a) {}

    virtual bool
    scatter(const ray &r_in, const hit_record &rec, glm::vec3 &attenuation, ray &scattered) const { return false; }

    virtual glm::vec3 emitted(const ray &r_in, const hit_record &rec, float u, float v, const glm::vec3 &p) const {
        if (dot(rec.normal, r_in.direction()) < 0.0f)
            return emit->value(u, v, p);
        else
            return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    texture *emit;
};

//class isotropic : public material {
//public:
//	isotropic(texture* a) : albedo(a) {}
//	virtual bool scatter(const ray& r_in, const hit_record& rec, glm::vec3& attenuation, ray& scattered) const {
//		scattered = ray(rec.p, random_in_unit_sphere());
//		attenuation = albedo->value(rec.u, rec.v, rec.p);
//		return true;
//	}
//	texture* albedo;
//};

#endif // !MATERIALH
