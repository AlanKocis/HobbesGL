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

void Shader::setFloat(std::string& uniform, const float i)
{
	glUniform1f(glGetUniformLocation(programID, uniform.c_str()), (float)i);

}

void Shader::setVec3(const char* uniform, const glm::vec3& vec)
{
	glUniform3fv(glGetUniformLocation(programID, uniform), 1, glm::value_ptr(vec));
}

void Shader::setVec4(const char* uniform, const glm::vec4& vec)
{
	glUniform4fv(glGetUniformLocation(programID, uniform), 1, glm::value_ptr(vec));
}

void Shader::setVec3(std::string& uniform, const glm::vec3& vec)
{
	glUniform3fv(glGetUniformLocation(programID, uniform.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec4(std::string& uniform, const glm::vec4& vec)
{
	glUniform4fv(glGetUniformLocation(programID, uniform.c_str()), 1, glm::value_ptr(vec));
}
void Shader::setMat4(const char* uniform, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat3(const char* uniform, const glm::mat3& matrix)
{
	glUniformMatrix3fv(glGetUniformLocation(programID, uniform), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat4(std::string& uniform, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, uniform.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat3(std::string& uniform, const glm::mat3& matrix)
{
	glUniformMatrix3fv(glGetUniformLocation(programID, uniform.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

