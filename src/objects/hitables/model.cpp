#include "model.hpp"

glm::vec3 model::random(const glm::vec3 &o) const {
    float r = random_number();
    int index = int(r * meshes.size());
    return meshes[index].random(o);
}

bool model::hit(const ray &r, float tmin, float tmax, hit_record &rec) const {

    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = tmax;
    for (int i = 0; i < meshes.size(); i++) {
        if (meshes[i].hit(r, tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool model::bounding_box(float t0, float t1, aabb &box) const {
    if (meshes.empty()) return false;
    aabb temp_box;
    bool first_true = meshes[0].bounding_box(t0, t1, temp_box);
    if (!first_true) {
        return false;
    } else {
        box = temp_box;
    }
    for (int i = 0; i < meshes.size(); i++) {
        if (meshes[i].bounding_box(t0, t1, temp_box)) {
            box = surrounding_box(box, temp_box);
        } else {
            return false;
        }
    }
    return true;
}

void model::loadModel(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

mesh model::processMesh(aiMesh *aMesh, const aiScene *scene) {
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < aMesh->mNumVertices; i++) {
        glm::vec3 vectorPoint;
        vectorPoint.x = aMesh->mVertices[i].x;
        vectorPoint.y = aMesh->mVertices[i].y;
        vectorPoint.z = aMesh->mVertices[i].z;
        glm::vec3 vectorNormal;
        vectorNormal.x = aMesh->mNormals[i].x;
        vectorNormal.y = aMesh->mNormals[i].y;
        vectorNormal.z = aMesh->mNormals[i].z;
        float u = 0, v = 0;
        if (aMesh->mTextureCoords[0]) {
            u = aMesh->mTextureCoords[0][i].x;
            v = aMesh->mTextureCoords[0][i].y;
        }

        vertices.emplace_back(vertex(vectorPoint, vectorNormal, u, v));
    }

    for (unsigned int i = 0; i < aMesh->mNumFaces; i++) {
        aiFace face = aMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return mesh(vertices, indices, mat_ptr);
}