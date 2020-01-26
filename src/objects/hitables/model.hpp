#pragma once
#ifndef MODELH

#include "hitable.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include <iostream>

class model : public hitable {

public:

	model(std::string path, material* m) : mat_ptr(m) {
		loadModel(path);
	}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	virtual vec3 random(const vec3& o) const;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	mesh processMesh(aiMesh* mesh, const aiScene* scene);

	material* mat_ptr;
	std::vector<mesh> meshes;
	std::string directory;
};

#endif // !MODELH
