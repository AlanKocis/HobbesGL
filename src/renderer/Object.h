#pragma once
#include "renderer/Model.h"
#include "renderer/Mesh.h"
#include "renderer/Shader.h"
#include "glm/glm.hpp"

struct Object
{
	Model* m_model;
	Shader* m_shader;
	glm::mat4 m_world;
	char m_toBeRemoved;
};