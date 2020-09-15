#pragma once
#define _USE_MATH_DEFINES

#include <math.h>
#include "util/math/vec3.hpp"
#include "util/math/onb.hpp"
#include "util/rng/random_number.hpp"
#include "objects/hitables/hitable.hpp"
#include <glm/glm.hpp>

class pdf {
public:
    virtual float value(const glm::vec3 &direction) const = 0;

    virtual glm::vec3 generate() const = 0;
};

class cosine_pdf : public pdf {
public:
    cosine_pdf(const glm::vec3 &w) { uvw.build_from_w(w); }

    virtual float value(const glm::vec3 &direction) const {
//		float cosine = dot(unit_vector(direction), uvw.w());
        float cosine = dot(glm::normalize(direction), uvw.w());
        if (cosine > 0.0f) {
            return cosine / (float) M_PI;
        } else {
            return 0;
        }
    }

    virtual glm::vec3 generate() const {
        return uvw.local(random_cosine_direction());
    }

    onb uvw;
};

class hitable_pdf : public pdf {
public:
    hitable_pdf(hitable *p, const glm::vec3 &origin) : ptr(p), o(origin) {}

    virtual float value(const glm::vec3 &direction) const {
        return ptr->pdf_value(o, direction);
    }

    virtual glm::vec3 generate() const {
        return ptr->random(o);
    }

    glm::vec3 o;
    hitable *ptr;
};

class mixture_pdf : public pdf {
public:
    mixture_pdf(pdf *p0, pdf *p1) {
        p[0] = p0;
        p[1] = p1;
    }

    virtual float value(const glm::vec3 &direction) const {
        return 0.5f * p[0]->value(direction) + 0.5f * p[1]->value(direction);
    }

    virtual glm::vec3 generate() const {
        if (random_number() < 0.5f) {
            return p[0]->generate();
        } else {
            return p[1]->generate();
        }
    }

    pdf *p[2];
};