#pragma once
#include "renderer/Shader.h"
#include <renderer/Model.h>
#include <renderer/Texture2D.h>
#include <renderer/Mesh.h>

struct Transform3D
{
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);

	glm::mat4 getTransformMat4() const
	{
		glm::mat4 t(1.0f);
		t = glm::scale(t, scale);
		t *= rot;
		t = glm::translate(t, pos * scale);
		return t;
	}
};

class Object
{
public:
	Shader* m_shader;
	Model* m_model;
	Mesh* m_mesh;

	Transform3D m_transform;

	Object()
	{
		m_shader = nullptr;
		m_model = nullptr;
		m_mesh = nullptr;
	}

	Object(Shader* shader, Model* model)
	{
		m_shader = shader;
		m_model = model;
		m_mesh = nullptr;
	}
	Object(Shader* shader, Mesh* mesh)
	{
		m_shader = shader;
		m_model = nullptr;
		m_mesh = mesh;
	}

};

