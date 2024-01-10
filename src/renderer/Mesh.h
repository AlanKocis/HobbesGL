#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//assimp

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

enum TEX_TYPES
{
	diffuse,
	specular
};

struct Texture
{
	unsigned int id;
	int type;	
};


struct Mesh
{
	uint32_t m_vao, m_vbo, m_ebo;

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	std::vector<Texture> m_textures;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures);
	void initMesh();

};