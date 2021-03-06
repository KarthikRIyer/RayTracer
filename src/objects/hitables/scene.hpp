#pragma once

#include "hitable_list.hpp"
#include "../camera/camera.hpp"
#include "../../render_process/render_settings.hpp"

class Scene {
public:

    Scene() {}

    Scene(Camera camera, hitable *world) {
        this->camera = camera;
        this->world = world;
    }

    void setLightHitables(hitable_list *lightHitableList) {
        this->lightHitableList = lightHitableList;
    }

    hitable_list *getLightHitableList() {
        return lightHitableList;
    }

    Camera camera;
    hitable *world;
    static const int GRADIENT_SKY = 0;
    static const int BLACK_SKY = 1;
    int SKY = GRADIENT_SKY;
private:
    hitable_list *lightHitableList;

};