#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "renderer/Texture2D.h"
#include <glm/gtc/matrix_transform.hpp>
//assimp

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};


struct Mesh
{
	uint32_t m_vao, m_vbo, m_ebo;

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	std::vector<Texture2D> m_textures;


	Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture2D>& textures);
	Mesh(const std::vector<float>& vertices, const Texture2D& texture);
	void initMesh();
	static Mesh* genCreateQuadMesh(const float& x, const float& y, const float& z);
};