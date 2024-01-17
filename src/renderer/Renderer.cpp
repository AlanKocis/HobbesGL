#include "renderer/Renderer.h"
#include <glm/gtc/matrix_transform.hpp>


void Renderer::drawMesh(const Mesh* mesh, Shader* shader)
{
	glBindVertexArray(mesh->m_vao);
	shader->use();

	for (int i = 0; i < mesh->m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, mesh->m_textures[i].mID);

		switch (mesh->m_textures[i].mType)
		{
		case DIFFUSE:
			shader->setInt("diffuse", i);
			break;
		case SPECULAR:
			shader->setInt("specular", i);
			break;
		}
	}



	glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::drawModel(Model* model, Shader* shader)
{

	for (const Mesh* mesh : model->m_meshes)
	{
		drawMesh(mesh, shader);
	}

}

void Renderer::renderScene(Scene* scene)
{
	for (Object* o : scene->m_objects)
	{
		o->m_shader->use();
		o->m_shader->setMat4("view", scene->m_view);
		o->m_shader->setMat4("proj", scene->m_projection);
		o->m_shader->setMat4("world", o->m_world);
		glm::mat4 normalTransform = glm::transpose(glm::inverse(scene->m_view * o->m_world));
		o->m_shader->setMat4("normalTransform", normalTransform);

		for (Mesh* m : o->m_model->m_meshes)
		{
			drawMesh(m, o->m_shader);
		}
	}




}
