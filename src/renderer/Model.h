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
#include "renderer/Texture2D.h"

struct Model
{
	std::vector<Texture2D> loaded_textures;
	std::vector<Mesh> m_meshes;

	Model(std::string filePath);
	void processNodeRecursive(aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
};