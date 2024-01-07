#pragma once
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "helpers/lights.h"
class Shader
{
private:
	unsigned int programID;
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	const unsigned int getID();
	void use();
	void setInt(const char* uniform, const int i);
	void setFloat(const char* uniform, const float i);
	void setVec3(const char* uniform, const glm::vec3& vec);
	void setVec4(const char* uniform, const glm::vec4& vec);
	void setMat4(const char* uniform, const glm::mat4& matrix);
	void setMat3(const char* uniform, const glm::mat3& matrix);
	void setDirLight(std::string uniform, const dirLight& dirLight);
	void setPointLight(std::string uniform, const pointLight& pointLight);
	void setSpotLight(std::string uniform, const spotLight& spotLight);

};

