#include "renderer/light.h"

DirLight::DirLight()
{
	m_diffuse = glm::vec3(1.0f);
	m_ambient = glm::vec3(0.3f);
	m_specular = glm::vec3(1.0f);
	m_lightDirection = glm::vec4(0.0f, -1.0f, -0.5f, 0.0f);
}

DirLight::DirLight(const glm::vec3& diff, const glm::vec3& ambient, const glm::vec3& specular, const glm::vec3& dir)
{
	m_diffuse = diff;
	m_ambient = ambient;
	m_specular = specular;
	m_lightDirection = glm::vec4(dir, 0.0f);
}

PointLight::PointLight()
{
	m_diffuse = glm::vec3(1.0f);
	m_ambient = glm::vec3(1.0f);
	m_specular = glm::vec3(1.0f);
	m_lightPosition = glm::vec4(10.0f, 3.0f, 0.0f, 1.0f);
	m_attC = 1.0f;
	m_attL = 0.09f;
	m_attQ = 0.032f;
}

PointLight::PointLight(const glm::vec3& diff, const glm::vec3& ambient, const glm::vec3& specular, const glm::vec3& pos)
{
	m_diffuse = glm::vec3(1.0f);
	m_ambient = glm::vec3(1.0f);
	m_specular = glm::vec3(1.0f);
	m_lightPosition = glm::vec4(pos, 1.0f);
	m_attC = 1.0f;
	m_attL = 0.09f;
	m_attQ = 0.032f;

}

SpotLight::SpotLight()
{
	m_diffuse = glm::vec3(4.0f);
	m_ambient = glm::vec3(0.1f);
	m_specular = glm::vec3(1.0f);
	m_lightPosition = glm::vec4(0.0f, 10.0f, 0.0f, 1.0f);
	m_lightDirection = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

	m_attC = 1.0f;
	m_attL = 0.09f;
	m_attQ = 0.032f;
}

SpotLight::SpotLight(const glm::vec3& diff, const glm::vec3& ambient, const glm::vec3& specular, const glm::vec3& pos, const glm::vec3& dir)
{
	m_diffuse = diff;
	m_ambient = ambient;
	m_specular = specular;
	m_lightPosition = glm::vec4(pos, 1.0f);
	m_lightDirection = glm::vec4(dir, 0.0f);

	m_attC = 1.0f;
	m_attL = 0.09f;
	m_attQ = 0.032f;
}

void DirLight::setAllUniforms(Shader* shader, const int& index, const glm::mat4& view)
{
	shader->use();
	shader->setVec3("directional_lights[" + std::to_string(index) + "].lightDir", view * m_lightDirection);
	shader->setVec3("directional_lights[" + std::to_string(index) + "].ambient", m_ambient);
	shader->setVec3("directional_lights[" + std::to_string(index) + "].diffuse", m_diffuse);
	shader->setVec3("directional_lights[" + std::to_string(index) + "].specular", m_specular);
}

void DirLight::updateUniforms(Shader* shader, const int& index, const glm::mat4& view)
{
	shader->use();
	shader->setVec3("directional_lights[" + std::to_string(index) + "].lightDir", view * m_lightDirection);
}

void PointLight::setAllUniforms(Shader* shader, const int& index, const glm::mat4& view)
{
	shader->use();
	shader->setVec3("point_lights[" + std::to_string(index) + "].lightPos", view * m_lightPosition);
	shader->setVec3("point_lights[" + std::to_string(index) + "].ambient", m_ambient);
	shader->setVec3("point_lights[" + std::to_string(index) + "].diffuse", m_diffuse);
	shader->setVec3("point_lights[" + std::to_string(index) + "].specular", m_specular);
	shader->setFloat("point_lights[" + std::to_string(index) + "].attC", m_attC);
	shader->setFloat("point_lights[" + std::to_string(index) + "].attL", m_attL);
	shader->setFloat("point_lights[" + std::to_string(index) + "].attQ", m_attQ);


}

void PointLight::updateUniforms(Shader* shader, const int& index, const glm::mat4& view)
{
	shader->use();
	shader->setVec3("point_lights[" + std::to_string(index) + "].lightPos", view * m_lightPosition);
}


void SpotLight::setAllUniforms(Shader* shader, const int& index, const glm::mat4& view)
{
	glm::vec3 pos = view * m_lightPosition;
	glm::vec3 dir = view * m_lightDirection;

	shader->use();
	shader->setVec3("spot_lights[" + std::to_string(index) + "].lightPos", pos);
	shader->setVec3("spot_lights[" + std::to_string(index) + "].direction", dir);
	shader->setVec3("spot_lights[" + std::to_string(index) + "].ambient", m_ambient);
	shader->setVec3("spot_lights[" + std::to_string(index) + "].diffuse", m_diffuse);
	shader->setVec3("spot_lights[" + std::to_string(index) + "].specular", m_specular);
	shader->setFloat("spot_lights[" + std::to_string(index) + "].attC", m_attC);
	shader->setFloat("spot_lights[" + std::to_string(index) + "].attL", m_attL);
	shader->setFloat("spot_lights[" + std::to_string(index) + "].attQ", m_attQ);
	shader->setFloat("spot_lights[" + std::to_string(index) + "].innerRadius", glm::cos(glm::radians(12.5f)));
	shader->setFloat("spot_lights[" + std::to_string(index) + "].outerRadius", glm::cos(glm::radians(15.5f)));


}

void SpotLight::updateUniforms(Shader* shader, const int& index, const glm::mat4& view)
{
	shader->use();
	shader->setVec3("spot_lights[" + std::to_string(index) + "].lightPos", view * m_lightPosition);
	shader->setVec3("spot_lights[" + std::to_string(index) + "].direction", view * m_lightDirection);
}

LightType DirLight::getType() { return DIR; }
LightType PointLight::getType() { return POINT; }
LightType SpotLight::getType() { return SPOT; }