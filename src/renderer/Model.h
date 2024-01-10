#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"  

#include <vector>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/Mesh.h"
#include <string>
#include <helpers/RootDir.h>
#include <iostream>

struct Model
{
	std::vector<Mesh> m_meshes;
	//char* m_path;

	Model(std::string filePath);
	void processNodeRecursive(aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
};