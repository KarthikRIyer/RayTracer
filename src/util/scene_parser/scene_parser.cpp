#include "scene_parser.hpp"

SceneParser::SceneParser(std::string path) : path(path) {
    std::ifstream file(path);
    if (!file){
        printf("Unable to open file!\n");
        exit(0);
    }
//    file >> JSONObject;
    try {
        JSONObject = json::parse(file);
    } catch (json::parse_error &e) {
        std::cerr << e.what() << std::endl;
        exit(0);
    }
//    exit(0);
    parseScene();
}

int SceneParser::getRenderSamples() {
    return renderSamples;
}

int SceneParser::getImageWidth() {
    return imageWidth;
}

int SceneParser::getImageHeight() {
    return imageHeight;
}

bool SceneParser::denoiseImage() {
    return denoise;
}

Scene *SceneParser::getScene() {
    return &scene;
}

texture *SceneParser::parseTexture(json textureJSON) {

    texture *currentTexture = NULL;
    if (textureJSON["type"] == "constantTexture") {
        currentTexture = new constant_texture(
                glm::vec3(textureJSON["color"][0], textureJSON["color"][1], textureJSON["color"][2]));
    } else if (textureJSON["type"] == "imageTexture") {
        currentTexture = new image_texture(textureJSON["path"]);
    } else if (textureJSON["type"] == "noiseTexture") {
        currentTexture = new noise_texture(textureJSON["noiseScale"]);
    } else if (textureJSON["type"] == "checkerTexture") {
        texture *t1 = parseTexture(textureJSON["texture1"]);
        texture *t2 = parseTexture(textureJSON["texture2"]);
        currentTexture = new checker_texture(t1, t2);
    }
    return currentTexture;
}

material *SceneParser::parseMaterial(json materialJSON) {
    material *currentMaterial = NULL;
    if (materialJSON["type"] == "lambertian") {
        texture *currentTexture = parseTexture(materialJSON["texture"]);
        currentMaterial = new lambertian(currentTexture);
    } else if (materialJSON["type"] == "diffuseLight") {
        texture *currentTexture = parseTexture(materialJSON["texture"]);
        currentMaterial = new diffuse_light(currentTexture);
    } else if (materialJSON["type"] == "metal") {
        currentMaterial = new metal(
                glm::vec3(materialJSON["color"][0], materialJSON["color"][1], materialJSON["color"][2]),
                materialJSON["fuzz"]);
    } else if (materialJSON["type"] == "dielectric") {
        currentMaterial = new dielectric(materialJSON["refractiveIndex"]);
    }
    return currentMaterial;
}

void SceneParser::buildMaterialList() {

    // build Material list
    materialCount = JSONObject["materialList"].size();
    materialList = new material *[materialCount];
    unsigned int count = 0;
    for (json::iterator it = JSONObject["materialList"].begin();
         it != JSONObject["materialList"].end(); ++it, ++count) {
        materialList[count] = parseMaterial(it.value());
    }

}

hitable *SceneParser::parseObject(json objectJSON) {

    // get basic object
    hitable *currentObject = NULL;
    int materialIndex = objectJSON["materialIndex"];
    if (objectJSON["type"] == "yzRect") {
        currentObject = new yz_rect(
                objectJSON["coords"][0],
                objectJSON["coords"][1],
                objectJSON["coords"][2],
                objectJSON["coords"][3],
                objectJSON["coords"][4],
                materialList[materialIndex]);
    } else if (objectJSON["type"] == "xzRect") {
        currentObject = new xz_rect(
                objectJSON["coords"][0],
                objectJSON["coords"][1],
                objectJSON["coords"][2],
                objectJSON["coords"][3],
                objectJSON["coords"][4],
                materialList[materialIndex]);
    } else if (objectJSON["type"] == "xyRect") {
        currentObject = new xy_rect(
                objectJSON["coords"][0],
                objectJSON["coords"][1],
                objectJSON["coords"][2],
                objectJSON["coords"][3],
                objectJSON["coords"][4],
                materialList[materialIndex]);
    } else if (objectJSON["type"] == "sphere") {
        currentObject = new sphere(
                glm::vec3(objectJSON["center"][0], objectJSON["center"][1], objectJSON["center"][2]),
                objectJSON["radius"],
                materialList[materialIndex]);
    } else if (objectJSON["type"] == "sphere") {
        currentObject = new sphere(
                glm::vec3(objectJSON["center"][0], objectJSON["center"][1], objectJSON["center"][2]),
                objectJSON["radius"],
                materialList[materialIndex]);
    } else if (objectJSON["type"] == "triangle") {
        currentObject = new triangle(
                vertex(
                        glm::vec3(objectJSON["v1"][0], objectJSON["v1"][1], objectJSON["v1"][2]),
                        glm::vec3(objectJSON["v1"][3], objectJSON["v1"][4], objectJSON["v1"][5]),
                        objectJSON["v1"][6], objectJSON["v1"][7]
                ),
                vertex(
                        glm::vec3(objectJSON["v2"][0], objectJSON["v2"][1], objectJSON["v2"][2]),
                        glm::vec3(objectJSON["v2"][3], objectJSON["v2"][4], objectJSON["v2"][5]),
                        objectJSON["v2"][6], objectJSON["v2"][7]
                ),
                vertex(
                        glm::vec3(objectJSON["v3"][0], objectJSON["v3"][1], objectJSON["v3"][2]),
                        glm::vec3(objectJSON["v3"][3], objectJSON["v3"][4], objectJSON["v3"][5]),
                        objectJSON["v3"][6], objectJSON["v3"][7]
                ),
                materialList[materialIndex]);
    } else if (objectJSON["type"] == "box") {
        currentObject = new box(
                glm::vec3(objectJSON["p0"][0], objectJSON["p0"][1], objectJSON["p0"][2]),
                glm::vec3(objectJSON["p1"][0], objectJSON["p1"][1], objectJSON["p1"][2]),
                materialList[materialIndex]);
    } else if (objectJSON["type"] == "model") {
        currentObject = new model(
                objectJSON["path"],
                materialList[materialIndex]);
    }

    // apply transformations of necessary
    float scaleValue = objectJSON["scale"];
    glm::vec3 objectLocation(objectJSON["location"][0], objectJSON["location"][1], objectJSON["location"][2]);
    float xRotation = objectJSON["rotation"][0];
    float yRotation = objectJSON["rotation"][1];
    float zRotation = objectJSON["rotation"][2];
    bool flipNormals = objectJSON["flipNormals"];

    if (scaleValue != 1) {
        currentObject = new scale(currentObject, scaleValue);
    }
    if (flipNormals) {
        currentObject = new flip_normals(currentObject);
    }
    if (xRotation != 0) {
        currentObject = new rotate_x(currentObject, xRotation);
    }
    if (yRotation != 0) {
        currentObject = new rotate_y(currentObject, yRotation);
    }
    if (zRotation != 0) {
        currentObject = new rotate_z(currentObject, zRotation);
    }
    if (objectLocation.x != 0 || objectLocation.y != 0 || objectLocation.z != 0) {
        currentObject = new translate(currentObject, objectLocation);
    }

    return currentObject;
}

void SceneParser::buildObjectList() {

    //build Object list
    objectCount = JSONObject["objectList"].size();
    objectList = new hitable *[objectCount];
    unsigned int count = 0;
    for (json::iterator it = JSONObject["objectList"].begin(); it != JSONObject["objectList"].end(); ++it, ++count) {
        objectList[count] = parseObject(it.value());
    }

}

void SceneParser::buildLightsVector() {
    lightsVector.clear();
    if (JSONObject["lightsList"].dump() == "null") {
        return;
    }

    std::vector<int> lightsIndexVector = JSONObject["lightsList"];
    for (unsigned int i = 0; i < lightsIndexVector.size(); i++) {
        lightsVector.push_back(objectList[lightsIndexVector[i]]);
    }
}

void SceneParser::parseScene() {

    renderSamples = JSONObject["renderSamples"];
    imageWidth = JSONObject["imageWidth"];
    imageHeight = JSONObject["imageHeight"];
    float verticalFieldOfView = JSONObject["camera"]["verticalFieldOfView"];
    float aperture = JSONObject["camera"]["aperture"];
    glm::vec3 lookfrom(JSONObject["camera"]["lookfrom"][0], JSONObject["camera"]["lookfrom"][1],
                       JSONObject["camera"]["lookfrom"][2]);
    glm::vec3 lookat(JSONObject["camera"]["lookat"][0], JSONObject["camera"]["lookat"][1],
                     JSONObject["camera"]["lookat"][2]);
    float focalDist = JSONObject["camera"]["focalDist"];
    Camera cam(lookfrom, lookat, glm::vec3(0, 1, 0), verticalFieldOfView, float(imageWidth) / float(imageHeight),
               aperture, focalDist, 0.0, 1.0);
    scene.camera = cam;
    scene.SKY = JSONObject["sky"];
    denoise = JSONObject["denoise"];
    buildMaterialList();
    buildObjectList();
    buildLightsVector();

    scene.world = new hitable_list(objectList, objectCount);
    lightHitableList = hitable_list(&lightsVector[0], lightsVector.size());
    scene.setLightHitables(&lightHitableList);
}