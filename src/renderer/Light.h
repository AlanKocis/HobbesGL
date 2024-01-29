#pragma once

#include "renderer/Shader.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer/Camera.h"

enum LightType
{
	DIR,
	POINT,
	SPOT
};

class Light
{
public:
	glm::vec3 m_diffuse;
	glm::vec3 m_ambient;
	glm::vec3 m_specular;

	virtual void setAllUniforms(Shader* shader, const int& index, const glm::mat4& view) = 0;
	virtual void updateUniforms(Shader* shader, const int& index, const glm::mat4& view) = 0;
	virtual LightType getType() = 0;

	virtual ~Light() {}

};

class DirLight : public Light
{
public:
	glm::vec4 m_lightDirection;
	void setAllUniforms(Shader* shader, const int& index, const glm::mat4& view);
	void updateUniforms(Shader* shader, const int& index, const glm::mat4& view);
	LightType getType();

	DirLight();
	DirLight(const glm::vec3& diff, const glm::vec3& ambient, const glm::vec3& specular, const glm::vec3& dir);
	~DirLight() {}
};

class PointLight : public Light
{
public:
	glm::vec4 m_lightPosition;
	float m_attC;
	float m_attL;
	float m_attQ;

	void setAllUniforms(Shader* shader, const int& index, const glm::mat4& view);
	void updateUniforms(Shader* shader, const int& index, const glm::mat4& view);
	LightType getType();

	PointLight();
	PointLight(const glm::vec3& diff, const glm::vec3& ambient, const glm::vec3& specular, const glm::vec3& pos);
	~PointLight() {}
};

class SpotLight : public Light
{
public:
	glm::vec4 m_lightPosition;
	glm::vec4 m_lightDirection;
	float m_attC;
	float m_attL;
	float m_attQ;
	void setAllUniforms(Shader* shader, const int& index, const glm::mat4& view);
	void updateUniforms(Shader* shader, const int& index, const glm::mat4& view);
	LightType getType();

	SpotLight();
	SpotLight(const glm::vec3& diff, const glm::vec3& ambient, const glm::vec3& specular, const glm::vec3& pos, const glm::vec3& dir);
	~SpotLight() {}
};
