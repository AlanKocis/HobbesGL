#include "renderer/Model.h"

Model::Model(std::string filePath)
{
	std::string path = ROOT_DIR "res/models/" + filePath;

	Assimp::Importer importer;
	const aiScene* ass_scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!ass_scene || ass_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ass_scene->mRootNode)
	{
		std::cout << "Error at " << path << ' ' << importer.GetErrorString() << std::endl;
		return;
	}

	processNodeRecursive(ass_scene->mRootNode, ass_scene);

	std::cout << "Loaded " << filePath << std::endl;
}

void Model::processNodeRecursive(aiNode* node, const aiScene* scene)
{
	//process all of this node's meshes:
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}


	//process each child node
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		processNodeRecursive(node->mChildren[i], scene);
	}

}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture2D> textures;



	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vert;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vert.position = vector;
		

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vert.normal = vector;
		}

		glm::vec2 texCoord(0.0f, 0.0f);
		if (mesh->mTextureCoords[0])
		{
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
		}
		vert.texCoord = texCoord;

		vertices.push_back(vert);

	}

	// a mesh's faces (primitives) contain indices for our element buffer object
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	//load textures
	int num_diffuse = material->GetTextureCount(aiTextureType_DIFFUSE);
	//std::cout << "number diffuse: " << num_diffuse << std::endl;

	for (int i = 0; i < num_diffuse; i++)
	{
		bool skip = false;
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, i, &path);
		//todo: check with vector of loaded textures to skip loading a texture twice
		//Texture2D tex(path.C_Str(), TEX_TYPES::DIFFUSE);

		for (int j = 0; j < loaded_textures.size(); j++)
		{
			if (std::strcmp(path.C_Str(),loaded_textures[j].mPath.c_str()) == 0)
			{
				skip = true;
			}
		}
		
		if (!skip)
		{
			Texture2D tex(path.C_Str(), TEX_TYPES::DIFFUSE);
			textures.push_back(tex);
			loaded_textures.push_back(tex);
		}

	}

	int num_specular = material->GetTextureCount(aiTextureType_SPECULAR);
	//std::cout << "number specular: " << num_specular << std::endl;

	for (unsigned int i = 0; i < num_specular; i++)
	{
		bool skip = false;
		aiString path;
		material->GetTexture(aiTextureType_SPECULAR, i, &path);
		for (int j = 0; j < loaded_textures.size(); j++)
		{
			if (std::strcmp(path.C_Str(), loaded_textures[j].mPath.c_str()) == 0)
			{
				skip = true;
			}
		}

		if (!skip)
		{
			Texture2D tex(path.C_Str(), TEX_TYPES::SPECULAR);
			textures.push_back(tex);
			loaded_textures.push_back(tex);
		}
	}



	


	return Mesh(vertices, indices, textures);
}


