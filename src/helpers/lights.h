#pragma once
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct dirLight
{
	glm::vec3 lightDir;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct pointLight
{
	glm::vec3 lightPos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float attC;
	float attL;
	float attQ;
};

struct spotLight
{
	glm::vec3 lightPos;
	glm::vec3 lightDir;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float innerRadius;
	float outerRadius;
	float attC;
	float attL;
	float attQ;
};
