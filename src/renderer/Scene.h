#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/Object.h"

struct Scene
{
	glm::mat4 m_view, m_projection;
	std::vector<Object*> m_objects;
};
