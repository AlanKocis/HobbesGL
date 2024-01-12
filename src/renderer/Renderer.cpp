#include "renderer/Renderer.h"


void Renderer::drawSolidMesh(const Mesh& mesh, Shader& shader)
{
	glBindVertexArray(mesh.m_vao);
	shader.use();

	for (int i = 0; i < mesh.m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh.m_textures[i].mID);

		switch (mesh.m_textures[i].mType)
		{
		case DIFFUSE:
			shader.setInt("diffuse", i);
			break;
		case SPECULAR:
			shader.setInt("specular", i);
			break;
		}
	}



	glDrawElements(GL_TRIANGLES, mesh.m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::drawSolidModel(Model& model, Shader& shader)
{

	for (const Mesh& mesh : model.m_meshes)
	{
		Renderer::drawSolidMesh(mesh, shader);
	}

}