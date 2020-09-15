#pragma once

#include "hitable.hpp"
#include "util/bounding_box/aabb.hpp"
#include "util/rng/random_number.hpp"

class xy_rect : public hitable {
public:
    xy_rect() {}

    xy_rect(float _x0, float _x1, float _y0, float _y1, float _k, material *mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1),
                                                                                   k(_k), mp(mat) {}

    virtual bool hit(const ray &r, float t0, float t1, hit_record &rec) const;

    virtual bool bounding_box(float t0, float t1, aabb &box) const {
        box = aabb(glm::vec3(x0, y0, k - 0.0001), glm::vec3(x1, y1, k - 0.0001));
        return true;
    }

    virtual float pdf_value(const glm::vec3 &o, const glm::vec3 &v) const {
        hit_record rec;
        if (this->hit(ray(o, v), 0.001f, FLT_MAX, rec)) {
            float area = (x1 - x0) * (y1 - y0);
            float distance_squared = rec.t * rec.t * glm::length2(v);
            float cosine = fabs(dot(v, rec.normal) / glm::length(v));
            return distance_squared / (cosine * area);
        } else {
            return 0;
        }
    }

    virtual glm::vec3 random(const glm::vec3 &o) const {
        glm::vec3 random_point = glm::vec3(x0 + random_number() * (x1 - x0), k, y0 + random_number() * (y1 - y0));
        return random_point - o;
    }

    material *mp;
    float x0, x1, y0, y1, k;
};

class xz_rect : public hitable {
public:
    xz_rect() {}

    xz_rect(float _x0, float _x1, float _z0, float _z1, float _k, material *mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1),
                                                                                   k(_k), mp(mat) {}

    virtual bool hit(const ray &r, float t0, float t1, hit_record &rec) const;

    virtual bool bounding_box(float t0, float t1, aabb &box) const {
        box = aabb(glm::vec3(x0, k - 0.0001, z0), glm::vec3(x1, k - 0.0001, z1));
        return true;
    }

    virtual float pdf_value(const glm::vec3 &o, const glm::vec3 &v) const {
        hit_record rec;
        if (this->hit(ray(o, v), 0.001f, FLT_MAX, rec)) {
            float area = (x1 - x0) * (z1 - z0);
            float distance_squared = rec.t * rec.t * glm::length2(v);
            float cosine = fabs(dot(v, rec.normal) / glm::length(v));
            return distance_squared / (cosine * area);
        } else {
            return 0;
        }
    }

    virtual glm::vec3 random(const glm::vec3 &o) const {
        glm::vec3 random_point = glm::vec3(x0 + random_number() * (x1 - x0), k, z0 + random_number() * (z1 - z0));
        return random_point - o;
    }

    material *mp;
    float x0, x1, z0, z1, k;
};

class yz_rect : public hitable {
public:
    yz_rect() {}

    yz_rect(float _y0, float _y1, float _z0, float _z1, float _k, material *mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1),
                                                                                   k(_k), mp(mat) {}

    virtual bool hit(const ray &r, float t0, float t1, hit_record &rec) const;

    virtual bool bounding_box(float t0, float t1, aabb &box) const {
        box = aabb(glm::vec3(k - 0.0001, y0, z0), glm::vec3(k - 0.0001, y1, z1));
        return true;
    }

    virtual float pdf_value(const glm::vec3 &o, const glm::vec3 &v) const {
        hit_record rec;
        if (this->hit(ray(o, v), 0.001f, FLT_MAX, rec)) {
            float area = (y1 - y0) * (z1 - z0);
            float distance_squared = rec.t * rec.t * glm::length2(v);
            float cosine = fabs(dot(v, rec.normal) / glm::length(v));
            return distance_squared / (cosine * area);
        } else {
            return 0;
        }
    }

    virtual glm::vec3 random(const glm::vec3 &o) const {
        glm::vec3 random_point = glm::vec3(y0 + random_number() * (y1 - y0), k, z0 + random_number() * (z1 - z0));
        return random_point - o;
    }

    material *mp;
    float y0, y1, z0, z1, k;
};