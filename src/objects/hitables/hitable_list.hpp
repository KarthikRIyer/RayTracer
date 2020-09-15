#pragma once
#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.hpp"
#include "util/rng/random_number.hpp"
#include<iostream>
#include<mutex>

class hitable_list : public hitable {
public:
    hitable_list() {}

    hitable_list(hitable **l, size_t n) {
        list = l;
        list_size = n;
    }

    //hitable_list(std::vector<hitable*> listVector) { list = &listVector[0]; list_size = listVector.size(); }
    virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;

    virtual bool bounding_box(float t0, float t1, aabb &box) const;

    virtual float pdf_value(const glm::vec3 &o, const glm::vec3 &v) const;

    virtual glm::vec3 random(const glm::vec3 &o) const;

    hitable **list;
    size_t list_size = 0;
};

#endif // !HITABLELISTH
