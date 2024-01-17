#pragma once
#include "renderer/Mesh.h"
#include "renderer/Model.h"
#include "renderer/Shader.h"
#include "renderer/Texture2D.h"
#include "renderer/Object.h"
#include "renderer/Scene.h"
#include <vector>


namespace Renderer
{
	void drawMesh(const Mesh* mesh, Shader* shader);
	void drawModel(Model* model, Shader* shader);
	void renderScene(Scene* scene);
}