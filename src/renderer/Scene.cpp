#include "renderer/Scene.h"

Scene::Scene()
{
	m_numDirLights = 0;
	m_numPointLights = 0;
	m_numSpotLights = 0;

	m_lights.reserve(16);
	m_objects.reserve(1000);
	m_currentCamera = nullptr;

}

void Scene::createAddLight(int type)
{
	Light* l;

	switch (type)
	{
	case DIR:
		l = new DirLight();
		m_numDirLights++;
		break;
	case POINT:
		l = new PointLight();
		m_numPointLights++;
		break;
	case SPOT:
		l = new SpotLight();
		m_numSpotLights++;
		break;
	}

	m_lights.push_back(l);
}

void Scene::addLight(Light* light)
{
	m_lights.push_back(light);
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
	for (Light* l : m_lights)
	{
		delete l;
	}
}

void Scene::addObject(Shader* shader, Model* model)
{
	m_objects.push_back(new Object(shader, model));
}

void Scene::addObject(Shader* shader, Mesh* mesh)
{
	m_objects.push_back(new Object(shader, mesh));
}

void Scene::freeAllObjects()
{
	for (Object* o : m_objects)
	{
		delete o;
	}
}