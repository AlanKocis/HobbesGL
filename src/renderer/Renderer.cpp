#pragma once
#include "renderer/Mesh.h"
#include "renderer/Model.h"
#include "renderer/Shader.h"

namespace Renderer
{
	static void drawSolidMesh(const Mesh& mesh, Shader& shader)
	{
		glBindVertexArray(mesh.m_vao);
		shader.use();
		glDrawElements(GL_TRIANGLES, mesh.m_indices.size(), GL_UNSIGNED_INT, 0);
	}


	static void drawSolidModel(Model& model, Shader& shader)
	{
		for (const Mesh& mesh : model.m_meshes)
		{
			Renderer::drawSolidMesh(mesh, shader);
		}

	}


}