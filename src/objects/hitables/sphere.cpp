#include "sphere.hpp"

// sphere functions
float sphere::pdf_value(const glm::vec3 &o, const glm::vec3 &v) const {
    hit_record rec;
    if (this->hit(ray(o, v), 0.001f, FLT_MAX, rec)) {
        float cos_theta_max = sqrt(1.0f - radius * radius / glm::length2(center - o));
        float solid_angle = 2.0f * (float) M_PI * (1.0f - cos_theta_max);
        return 1.0f / solid_angle;
    } else {
        return 0;
    }
}

glm::vec3 sphere::random(const glm::vec3 &o) const {
    glm::vec3 direction = center - o;
    float distance_squared = glm::length2(direction);
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
}

bool sphere::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {

    glm::vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(float t0, float t1, aabb &box) const {
    box = aabb(center - glm::vec3(radius, radius, radius), center + glm::vec3(radius, radius, radius));
    return true;
}

// moving_sphere functions

glm::vec3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {
    glm::vec3 oc = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            get_sphere_uv((rec.p - center(r.time())) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            get_sphere_uv((rec.p - center(r.time())) / radius, rec.u, rec.v);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb &box) const {
    aabb box0 = aabb(center(t0) - glm::vec3(radius, radius, radius), center(t0) + glm::vec3(radius, radius, radius));
    aabb box1 = aabb(center(t1) - glm::vec3(radius, radius, radius), center(t1) + glm::vec3(radius, radius, radius));
    box = surrounding_box(box0, box1);
    return true;
}