#include "renderer/Scene.h"
#define NDEBUG

Scene::Scene()
	: m_lightPool(1000 * sizeof(Light)), m_objectPool(1000 * sizeof(Object))
{
	m_numDirLights = 0;
	m_numPointLights = 0;
	m_numSpotLights = 0;
	m_deleteLightIndex = -1;

	m_lights.reserve(1000);
	m_objects.reserve(1000);

	m_currentCamera = nullptr;
	m_LightingShader = Shader("vert_normals.glsl", "phong_frag.glsl");

}

void Scene::createAddLight(int type)
{

	Light* l;
	void* address;
	switch (type)
	{
	case DIR:
		address = m_lightPool.alloc(sizeof(DirLight), DEFAULT_ALLIGNMENT);
		assert(address != NULL);
		l = new(address) DirLight();
		m_numDirLights++;
		break;
	case POINT:
		address = m_lightPool.alloc(sizeof(PointLight), DEFAULT_ALLIGNMENT);
		assert(address != NULL);
		l = new(address) PointLight();
		m_numPointLights++;
		break;
	case SPOT:
		address = m_lightPool.alloc(sizeof(SpotLight), DEFAULT_ALLIGNMENT);
		assert(address != NULL);
		l = new(address) SpotLight();
		m_numSpotLights++;
		break;
	}
	m_lights.push_back(l);
}

void Scene::addLight(Light*& light)
{
	Light* l = (Light*)m_lightPool.alloc(sizeof(Light), DEFAULT_ALLIGNMENT);
	*l = *light;

	assert(l);

	m_lights.push_back(l);
	switch (light->getType())
	{
	case DIR:
		m_numDirLights++;
		break;
	case LightType::POINT:
		m_numPointLights++;
		break;
	case LightType::SPOT:
		m_numSpotLights++;
		break;
	}
}

void Scene::freeAllLights()
{

	free(m_lightPool.buffer);

	//for (Light* l : m_lights)
	//{
	//	delete l;
	//}
}

void Scene::createAddObject(Shader* shader, Model* model)
{
	void* address = m_objectPool.alloc(sizeof(Object), DEFAULT_ALLIGNMENT);
	assert(address);
	Object* o = new(address) Object(shader, model);
	m_objects.push_back(o);
}

void Scene::createAddObject(Shader* shader, Mesh* mesh)
{
	void* address = m_objectPool.alloc(sizeof(Object), DEFAULT_ALLIGNMENT);
	assert(address);
	Object* o = new(address) Object(shader, mesh);
	m_objects.push_back(o);
}

void Scene::setDeleteLightIndex(const int& index)
{
	m_deleteLightIndex = index;
}



void Scene::freeAllObjects()
{
	free(m_objectPool.buffer);

	//for (Object* o : m_objects)
	//{
 //		delete o;
	//}
}

void Scene::updateScene()
{

	if (m_deleteLightIndex < 0)
		return;

	for (int i = 0; i < m_lights.size(); i++)
	{
		if (i == m_deleteLightIndex)
		{
			LightType type = m_lights[i]->getType();
			switch (type)
			{
			case DIR:
				m_numDirLights--;
				break;
			case SPOT:
				m_numSpotLights--;
				break;
			case POINT:
				m_numPointLights--;
				break;
			}
			//delete m_lights[i];
			(m_lights.begin() + i) = m_lights.erase(m_lights.begin() + i);
			break;
		}
	}

	//m_lights.pop_back();
	m_deleteLightIndex = -1;
}

