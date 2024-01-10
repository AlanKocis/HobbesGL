#include <glad/glad.h>
#include "renderer/Mesh.h"
#include  <cstddef>
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures)
{
	//data copied
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	initMesh();
}

void Mesh::initMesh()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);	//up here OK 

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//std::vectors guaranteed to be contiguous, but a pointer pointing to the first element could change via realloc() call. &v[0] is safe here.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

	//glBindVertexArray(m_vao);	//down here null dereference (??)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));	
	glEnableVertexAttribArray(2);

	//glBindVertexArray(0);

}