#include "renderer/Camera.h"
#include <iostream>

Camera::Camera(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up)
{
	c_position = pos;
	c_forward = forward;
	c_up = up;
	//updateVectors();
}

void Camera::updateVectors()
{
	c_forward = glm::normalize(c_forward);
	c_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), -c_forward));
	c_up = glm::cross(-c_forward, c_right);
}

void Camera::setForward(const glm::vec3& forward)
{
	c_forward = forward;
	updateVectors();
}

void Camera::processInput(GLFWwindow* window, const float& speed)
{

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		c_position += speed * c_forward;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		c_position -= speed * c_forward;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		c_position -= speed * glm::normalize(-glm::cross(c_up, c_forward));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		c_position += speed * glm::normalize(-glm::cross(c_up, c_forward));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		c_position.y += speed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		c_position.y -= speed;



	//std::cout << "cam pos: " << c_position.x << ' ' << c_position.y << ' ' << c_position.z << '\n';
	//std::cout << "cam forward: " << c_forward.x << ' ' << c_forward.y << ' ' << c_forward.z << '\n';

}



void Camera::updateCameraTransform()
{
	updateVectors();

	c_viewMat[0][0] = c_right.x;
	c_viewMat[0][1] = c_up.x;
	c_viewMat[0][2] = -c_forward.x;
	c_viewMat[0][3] = 0.0f;

	c_viewMat[1][0] = c_right.y;
	c_viewMat[1][1] = c_up.y;
	c_viewMat[1][2] = -c_forward.y;
	c_viewMat[1][3] = 0.0f;

	c_viewMat[2][0] = c_right.z;
	c_viewMat[2][1] = c_up.z;
	c_viewMat[2][2] = -c_forward.z;
	c_viewMat[2][3] = 0.0f;

	c_viewMat[3][0] = glm::dot(-c_position, c_right);
	c_viewMat[3][1] = glm::dot(-c_position, c_up);
	c_viewMat[3][2] = glm::dot(-c_position, -c_forward);
	c_viewMat[3][3] = 1.0f;
}
