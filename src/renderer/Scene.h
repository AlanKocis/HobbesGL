#pragma once
#include <vector>
#include "renderer/Model.h"
#include "renderer/Shader.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/Light.h"
#include "renderer/Object.h"
#include "renderer/Camera.h"

const int MAX_LIGHTS = 10;


class Scene
{
public:
	// make arena for lights, objects, etc so this stuff gets stored linearly. Then, storing pointers is still fine, just call custom alloc() instead of new/delete
	std::vector<Light*> m_lights;			
	std::vector<Object*> m_objects;
	Camera* m_currentCamera;

	int m_numDirLights;
	int m_numSpotLights;
	int m_numPointLights;

	Scene();
	void createAddLight(int type);
	void addLight(Light* light);
	void freeAllLights();
	//void setLightUniforms(Light* light);
	//void updateLightMats();
	void addObject(Shader* shader, Model* model);
	void addObject(Shader* shader, Mesh* mesh);
	void freeAllObjects();
};