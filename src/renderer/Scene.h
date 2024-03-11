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
#include "renderer/Shader.h"
#include "helpers/Pool.h"

const int MAX_LIGHTS = 10;


class Scene
{
public:
	// make arena for lights, objects, etc so this stuff gets stored linearly. Then, storing pointers is still fine, just call custom alloc() instead of new/delete
	std::vector<Light*> m_lights;			
	std::vector<Object*> m_objects;

	Pool m_lightPool;
	Pool m_objectPool;


	Camera* m_currentCamera;

	int m_numDirLights;
	int m_numSpotLights;
	int m_numPointLights;
	int m_deleteLightIndex;
	int m_deleteObjIndex;
	Shader m_LightingShader;

	Scene();
	void createAddLight(int type);
	void addLight(Light*& light);
	void freeAllLights();
	//void setLightUniforms(Light* light);
	//void updateLightMats();
	void createAddObject(Shader* shader, Model* model);
	void createAddObject(Shader* shader, Mesh* mesh);
	void setDeleteLightIndex(const int& index);
	void setDeleteObjIndex(const int& index);
	void freeAllObjects();
	void updateScene();
};