#include "texture.hpp"

#include <utility>

image_texture::image_texture(std::string path) {
    image = Image(std::move(path));
    data = image.getBuffer();
}

glm::vec3 image_texture::value(float u, float v, const glm::vec3 &p) const {
    int i = (int) (u * nX);
    int j = (int) ((1.0f - v) * nY - 0.001f);
    if (i < 0)i = 0;
    if (j < 0)j = 0;
    if (i > nX - 1)i = nX - 1;
    if (j > nY - 1)j = nY - 1;
    float r = data[3 * i + 3 * nX * j];
    float g = data[3 * i + 3 * nX * j + 1];
    float b = data[3 * i + 3 * nX * j + 2];
    return glm::vec3(r, g, b);
}