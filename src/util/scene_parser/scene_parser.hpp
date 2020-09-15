#pragma once

#include<fstream>
#include<string>
#include<vector>
#include<nlohmann/json.hpp>
#include <glm/glm.hpp>

#include "objects/hitables/allHitables.hpp"
#include "objects/camera/camera.hpp"
#include "materials/material.hpp"
#include "textures/perlin.hpp"
#include "textures/texture.hpp"

using json = nlohmann::json;

class SceneParser {
public:
    SceneParser(std::string path);

    int getRenderSamples();

    int getImageWidth();

    int getImageHeight();

    bool denoiseImage();

    Scene *getScene();

private:
    std::string path;
    json JSONObject;
    Scene scene;
    int renderSamples = 1;
    int imageWidth = 0;
    int imageHeight = 0;
    bool denoise = false;
    size_t materialCount = 0;
    size_t objectCount = 0;
    material **materialList;
    hitable **objectList;
    hitable_list lightHitableList;
    std::vector<hitable *> lightsVector;

    void parseScene();

    void buildMaterialList();

    void buildObjectList();

    void buildLightsVector();

    material *parseMaterial(json materialJSON);

    texture *parseTexture(json materialJSON);

    hitable *parseObject(json objectJSON);

};