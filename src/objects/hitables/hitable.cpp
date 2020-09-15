#include "hitable.hpp"

void get_sphere_uv(const glm::vec3 &p, float &u, float &v) {
    float phi = atan2(p.z, p.x);
    float theta = asin(p.y);
    u = 1.0f - (phi + (float) M_PI) / (2.0f * (float) M_PI);
    v = (theta + (float) M_PI / 2.0f) / (float) M_PI;
    //std::cout << u << " " << v << "\n";
}

// translate functions
bool translate::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (ptr->hit(moved_r, t_min, t_max, rec)) {
        rec.p += offset;
        return true;
    } else {
        return false;
    }
}

bool translate::bounding_box(float t0, float t1, aabb &box) const {
    if (ptr->bounding_box(t0, t1, box)) {
        box = aabb(box.min() + offset, box.max() + offset);
        return true;
    } else {
        return false;
    }
}

// rotate_y functions
rotate_y::rotate_y(hitable *p, float angle) : ptr(p) {
    float radians = ((float) M_PI / 180.0f) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);
    glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                float x = i * bbox.max().x + (1 - i) * bbox.min().x;
                float y = j * bbox.max().y + (1 - j) * bbox.min().y;
                float z = k * bbox.max().z + (1 - k) * bbox.min().z;
                float newx = cos_theta * x + sin_theta * z;
                float newz = -sin_theta * x + cos_theta * z;
                glm::vec3 tester(newx, y, newz);
                for (int c = 0; c < 3; c++) {
                    if (tester[c] > max[c]) {
                        max[c] = tester[c];
                    }
                    if (tester[c] < min[c]) {
                        min[c] = tester[c];
                    }
                }
            }
        }
    }
    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    glm::vec3 origin = r.origin();
    glm::vec3 direction = r.direction();
    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];
    ray rotated_r(origin, direction, r.time());
    if (ptr->hit(rotated_r, t_min, t_max, rec)) {
        glm::vec3 p = rec.p;
        glm::vec3 normal = rec.normal;
        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
        rec.p = p;
        rec.normal = normal;
        return true;
    } else {
        return false;
    }

}

// rotate_x functions
rotate_x::rotate_x(hitable *p, float angle) : ptr(p) {
    float radians = ((float) M_PI / 180.0f) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);
    glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                float x = i * bbox.max().x + (1 - i) * bbox.min().x;
                float y = j * bbox.max().y + (1 - j) * bbox.min().y;
                float z = k * bbox.max().z + (1 - k) * bbox.min().z;
                float newy = cos_theta * y + sin_theta * z;
                float newz = -sin_theta * y + cos_theta * z;
                glm::vec3 tester(x, newy, newz);
                for (int c = 0; c < 3; c++) {
                    if (tester[c] > max[c]) {
                        max[c] = tester[c];
                    }
                    if (tester[c] < min[c]) {
                        min[c] = tester[c];
                    }
                }
            }
        }
    }
    bbox = aabb(min, max);
}

bool rotate_x::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    glm::vec3 origin = r.origin();
    glm::vec3 direction = r.direction();
    origin[1] = cos_theta * r.origin()[1] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[1] + cos_theta * r.origin()[2];
    direction[1] = cos_theta * r.direction()[1] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[1] + cos_theta * r.direction()[2];
    ray rotated_r(origin, direction, r.time());
    if (ptr->hit(rotated_r, t_min, t_max, rec)) {
        glm::vec3 p = rec.p;
        glm::vec3 normal = rec.normal;
        p[1] = cos_theta * rec.p[1] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[1] + cos_theta * rec.p[2];
        normal[1] = cos_theta * rec.normal[1] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[1] + cos_theta * rec.normal[2];
        rec.p = p;
        rec.normal = normal;
        return true;
    } else {
        return false;
    }

}

// rotate_z functions
rotate_z::rotate_z(hitable *p, float angle) : ptr(p) {
    float radians = ((float) M_PI / 180.0f) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);
    glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                float x = i * bbox.max().x + (1 - i) * bbox.min().x;
                float y = j * bbox.max().y + (1 - j) * bbox.min().y;
                float z = k * bbox.max().z + (1 - k) * bbox.min().z;
                float newx = cos_theta * x + sin_theta * y;
                float newy = -sin_theta * x + cos_theta * y;
                glm::vec3 tester(newx, newy, z);
                for (int c = 0; c < 3; c++) {
                    if (tester[c] > max[c]) {
                        max[c] = tester[c];
                    }
                    if (tester[c] < min[c]) {
                        min[c] = tester[c];
                    }
                }
            }
        }
    }
    bbox = aabb(min, max);
}

bool rotate_z::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    glm::vec3 origin = r.origin();
    glm::vec3 direction = r.direction();
    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[1];
    origin[1] = sin_theta * r.origin()[0] + cos_theta * r.origin()[1];
    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[1];
    direction[1] = sin_theta * r.direction()[0] + cos_theta * r.direction()[1];
    ray rotated_r(origin, direction, r.time());
    if (ptr->hit(rotated_r, t_min, t_max, rec)) {
        glm::vec3 p = rec.p;
        glm::vec3 normal = rec.normal;
        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[1];
        p[1] = -sin_theta * rec.p[0] + cos_theta * rec.p[1];
        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[1];
        normal[1] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[1];
        rec.p = p;
        rec.normal = normal;
        return true;
    } else {
        return false;
    }

}

// scale functions
scale::scale(hitable *p, float s) : ptr(p), m_scale(s) {
    hasbox = ptr->bounding_box(0, 1, bbox);
    float minx = bbox.min().x * m_scale;
    float miny = bbox.min().y * m_scale;
    float minz = bbox.min().z * m_scale;
    float maxx = bbox.max().x * m_scale;
    float maxy = bbox.max().y * m_scale;
    float maxz = bbox.max().z * m_scale;
    glm::vec3 min(minx, miny, minz);
    glm::vec3 max(maxx, maxy, maxz);
    bbox = aabb(min, max);
    m_inv_scale = 1.0f / m_scale;
}

bool scale::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    glm::vec3 origin = r.origin() * m_inv_scale;
    glm::vec3 direction = r.direction() * m_inv_scale;
    ray scaled_r(origin, direction, r.time());
    if (ptr->hit(scaled_r, t_min, t_max, rec)) {
        glm::vec3 p = rec.p;
        glm::vec3 normal = rec.normal;
        p[0] = m_scale * rec.p[0];
        p[1] = m_scale * rec.p[1];
        p[2] = m_scale * rec.p[2];
        rec.p = p;
        rec.normal = normal;
        return true;
    } else {
        return false;
    }

}