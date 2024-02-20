#include <glad/glad.h>
#include "renderer/Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture2D>& textures)
{
	//data copied
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;
	initMesh();
}

Mesh::Mesh(const std::vector<float>& vertices, const Texture2D& texture)
{
	m_ebo = 0;

	//Vertex v;

	//for (int i = 0; i < vertices.size(); i += 8)
	//{
	//	v.position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
	//	v.normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
	//	v.texCoord = glm::vec2(vertices[i + 6], vertices[i + 7]);
	//	m_vertices.push_back(v);
	//}

	m_textures.push_back(texture);

	//glGenVertexArrays(1, &m_vao);
	//glGenBuffers(1, &m_vbo);

	//glBindVertexArray(m_vao);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	//glEnableVertexAttribArray(2);

	//glBindVertexArray(0);


	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	for (int i = 0; i < 36; i++)
		m_vertices.push_back(Vertex{});

}

void Mesh::initMesh()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	//binding vao before ebo binds the two together - otherwise bind ebo after vao when drawing.
	glBindVertexArray(m_vao);

	//std::vectors guaranteed to be contiguous, but a pointer pointing to the first element could change via realloc() call. &v[0] is safe here.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));	
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

}

Mesh* Mesh::genCreateQuadMesh(const float& x, const float& y, const float& z)
{

	Texture2D defaultTex("DEFAULT_TEX_WHITE.jpg", DIFFUSE);


	std::vector<Vertex> vertices{
		//front face
		//pos			normal				tex
		{glm::vec3(-x / 2, -y / 2, -z / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2, -y / 2, -z / 2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x/2, y/2, -z/2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x/2, y/2, -z/2), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},

		//back face
		{glm::vec3(-x / 2, -y / 2, -z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2, -y / 2, -z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, -z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, -z / 2), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},

		//left face
		{glm::vec3(-x / 2, -y / 2, -z / 2), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-x / 2, -y / 2, z / 2), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, z / 2), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, -z / 2), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

		//right face
		{glm::vec3(x / 2, -y / 2, -z / 2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},	
		{glm::vec3(x / 2, -y / 2, z / 2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, z / 2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, -z / 2), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

		//top face
		{glm::vec3(-x / 2, y / 2, -z / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, -z / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, y / 2, z / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, y / 2, z / 2), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

		//bottom face
		{glm::vec3(-x / 2, -y / 2, -z / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(x / 2, -y / 2, -z / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(x / 2, -y / 2, z / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-x / 2, -y / 2, z / 2), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},

	};

	std::vector<unsigned int> indices
	{
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,
	};

	std::vector<Texture2D> textures;
	textures.push_back(defaultTex);

	return new Mesh(vertices, indices, textures);

}
