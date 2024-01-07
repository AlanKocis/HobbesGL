#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
	glm::vec3 c_position;
	glm::vec3 c_forward;
	glm::vec3 c_up;
	glm::vec3 c_right;
	glm::mat4 c_viewMat;

	Camera(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up);

	void updateVectors();

	void setForward(const glm::vec3& forward);

	void processInput(GLFWwindow* window, const float& speed);

	void updateCameraTransform();


};