#include "lighting_integrator.hpp"

glm::vec3 LightIntegrator::trace(const ray &r, Scene *scene, hitable_list *light_shape, int depth) {

    hit_record hrec;
    if ((scene->world)->hit(r, 0.001f, FLT_MAX, hrec)) {
        scatter_record srec;
        glm::vec3 emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
        if (depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec)) {
            if (srec.is_specular) {
                return srec.attenuation * trace(srec.specular_ray, scene, light_shape, depth + 1);
            } else {
                if (light_shape->list_size == 0) {
                    cosine_pdf p(hrec.normal);
                    ray scattered = ray(hrec.p, p.generate(), r.time());
                    float pdf_val = p.value(scattered.direction());
                    delete srec.pdf_ptr;
                    return emitted + srec.attenuation * hrec.mat_ptr->scattering_pdf(r, hrec, scattered) *
                                     trace(scattered, scene, light_shape, depth + 1) / pdf_val;
                } else {
                    hitable_pdf plight(light_shape, hrec.p);
                    mixture_pdf p(&plight, srec.pdf_ptr);
                    ray scattered = ray(hrec.p, p.generate(), r.time());
                    float pdf_val = p.value(scattered.direction());
                    delete srec.pdf_ptr;
                    return emitted + srec.attenuation * hrec.mat_ptr->scattering_pdf(r, hrec, scattered) *
                                     trace(scattered, scene, light_shape, depth + 1) / pdf_val;
                }
            }
        } else {
            return emitted;
        }
    } else {
        if (scene->SKY == Scene::GRADIENT_SKY) {
            glm::vec3 unit_direction = glm::normalize(r.direction());
            float t = 0.5f * (unit_direction.y + 1.0f);
            return (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
        } else if (scene->SKY == Scene::BLACK_SKY) {
            return glm::vec3(0, 0, 0);
        } else {
            return glm::vec3(0, 0, 0);
        }
    }

}