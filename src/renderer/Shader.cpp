#include "Shader.h"
#include <helpers/RootDir.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ostringstream ss;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		std::string vertexStringPath = vertexPath;
		vertexStringPath = ROOT_DIR "res/shaders/" + vertexStringPath;
		std::string fragmentStringPath = fragmentPath;
		fragmentStringPath = ROOT_DIR "res/shaders/" + fragmentStringPath;
		vShaderFile.open(vertexStringPath);
		fShaderFile.open(fragmentStringPath);
		std::ostringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "error reading shader files: " << e.what() << std::endl;
	}

	const char* vertexCString = vertexCode.c_str();
	const char* fragmentCString = fragmentCode.c_str();

	int success;
	char errorLog[512];

	unsigned int vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderObject, 1, &vertexCString, NULL);
	glCompileShader(vertexShaderObject);
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderObject, 512, NULL, errorLog);
		std::cout << "error compiling vertex shader\n" << errorLog << std::endl;
	}

	unsigned int fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderObject, 1, &fragmentCString, NULL);
	glCompileShader(fragmentShaderObject);
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderObject, 512, NULL, errorLog);
		std::cout << "error compiling fragment shader\n" << errorLog << std::endl;
	}
	
	this->programID = glCreateProgram();
	glAttachShader(programID, vertexShaderObject);
	glAttachShader(programID, fragmentShaderObject);
	glLinkProgram(programID);
	glGetShaderiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(programID, 512, NULL, errorLog);
		std::cout << "error linking shader program\n" << errorLog << std::endl;
	}

	glDeleteShader(vertexShaderObject);
	glDeleteShader(fragmentShaderObject);

}

const unsigned int Shader::getID()
{
	return this->programID;
}

void Shader::use()
{
	glUseProgram(programID);
}

void Shader::setInt(const char* uniform, const int i)
{
	glUniform1i(glGetUniformLocation(programID, uniform), i);
}

void Shader::setFloat(const char* uniform, const float i)
{
	glUniform1f(glGetUniformLocation(programID, uniform), (float)i);
}

void Shader::setVec3(const char* uniform, const glm::vec3& vec)
{
	glUniform3fv(glGetUniformLocation(programID, uniform), 1, glm::value_ptr(vec));
}

void Shader::setVec4(const char* uniform, const glm::vec4& vec)
{
	glUniform4fv(glGetUniformLocation(programID, uniform), 1, glm::value_ptr(vec));
}

void Shader::setMat4(const char* uniform, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat3(const char* uniform, const glm::mat3& matrix)
{
	glUniformMatrix3fv(glGetUniformLocation(programID, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setDirLight(std::string uniform, const dirLight& dirLight)
{

	setVec3((uniform + ".lightDir").c_str(), dirLight.lightDir);
	setVec3((uniform + ".ambient").c_str(), dirLight.ambient);
	setVec3((uniform + ".diffuse").c_str(), dirLight.diffuse);
	setVec3((uniform + ".specular").c_str(), dirLight.specular);
}

void Shader::setPointLight(std::string uniform, const pointLight& pointLight)
{
	setVec3((uniform + ".lightPos").c_str(), pointLight.lightPos);
	setVec3((uniform + ".ambient").c_str(), pointLight.ambient);
	setVec3((uniform + ".diffuse").c_str(), pointLight.diffuse);
	setVec3((uniform + ".specular").c_str(), pointLight.specular);
	setFloat((uniform + ".attC").c_str(), pointLight.attC);
	setFloat((uniform + ".attL").c_str(), pointLight.attL);
	setFloat((uniform + ".attQ").c_str(), pointLight.attQ);
}

void Shader::setSpotLight(std::string uniform, const spotLight& spotLight)
{
	setVec3((uniform + ".lightPos").c_str(), spotLight.lightPos);
	setVec3((uniform + ".direction").c_str(), spotLight.lightDir);
	setVec3((uniform + ".ambient").c_str(), spotLight.ambient);
	setVec3((uniform + ".diffuse").c_str(), spotLight.diffuse);
	setVec3((uniform + ".specular").c_str(), spotLight.specular);

	setFloat((uniform + ".innerRadius").c_str(), spotLight.innerRadius);
	setFloat((uniform + ".outerRadius").c_str(), spotLight.outerRadius);

	setFloat((uniform + ".attC").c_str(), spotLight.attC);
	setFloat((uniform + ".attL").c_str(), spotLight.attL);
	setFloat((uniform + ".attQ").c_str(), spotLight.attQ);
}
