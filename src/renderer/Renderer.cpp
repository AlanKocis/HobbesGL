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
			shader->setInt("material.diffuse", i);
			break;
		case SPECULAR:
			shader->setInt("material.specular", i);
			break;
		}
	}


	if (mesh->m_ebo != 0)
		glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, mesh->m_vertices.size());
}

void Renderer::drawModel(Model* model, Shader* shader)
{

	for (const Mesh* mesh : model->m_meshes)
	{
		drawMesh(mesh, shader);
	}
}

void Renderer::drawScene(Scene* scene)
{
	int dirIndex, pointIndex, spotIndex;
	dirIndex = pointIndex = spotIndex = 0;
	glm::mat4 view = scene->m_currentCamera->genCameraMatrix();
	glm::mat4 proj = scene->m_currentCamera->genProjectionMatrix();

	for (Object* o : scene->m_objects)
	{
		for (Light* l : scene->m_lights)
		{
			switch (l->getType())
			{
			case DIR:
				l->setAllUniforms(o->m_shader, dirIndex++, view);
				break;
			case POINT:
				l->setAllUniforms(o->m_shader, pointIndex++, view);
				break;
			case SPOT:
				l->setAllUniforms(o->m_shader, spotIndex++, view);
				break;
			}
		}
		 
		o->m_shader->use();
		o->m_shader->setInt("NUM_DIR_LIGHTS", scene->m_numDirLights);
		o->m_shader->setInt("NUM_POINT_LIGHTS", scene->m_numPointLights);
		o->m_shader->setInt("NUM_SPOT_LIGHTS", scene->m_numSpotLights);
		o->m_shader->setFloat("material.shininess", 32.0f);

		o->m_shader->setMat4("view", view);
		o->m_shader->setMat4("proj", proj);
		o->m_shader->setMat4("world", o->m_transform.getTransformMat4());
		if (o->m_model != nullptr)
			Renderer::drawModel(o->m_model, o->m_shader);
		else if (o->m_mesh != nullptr)
			Renderer::drawMesh(o->m_mesh, o->m_shader);
	}

}
