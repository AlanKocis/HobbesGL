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
	std::vector<Texture> textures;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec2 texCoord(0.0f, 0.0f);
		//if (mesh->mTextureCoords[0])
		//{
			//texCoord.x = mesh->mTextureCoords[0][i].x;
			//texCoord.y = mesh->mTextureCoords[0][i].y;
		//}
		vertices.push_back(
			Vertex{
				glm::vec3((float)mesh->mVertices[i].x, (float)mesh->mVertices[i].y, (float)mesh->mVertices[i].z),
				glm::vec3((float)mesh->mNormals[i].x, (float)mesh->mNormals[i].y, (float)mesh->mNormals[i].z),
				texCoord}
		);

	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}


	return Mesh(vertices, indices, textures);
}
