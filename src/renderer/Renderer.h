#pragma once
#include "renderer/Mesh.h"
#include "renderer/Model.h"
#include "renderer/Shader.h"
#include "renderer/Texture2D.h"

#include <vector>


namespace Renderer
{
	void drawSolidMesh(const Mesh& mesh, Shader& shader);
	void drawSolidModel(Model& model, Shader& shader);
}