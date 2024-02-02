#include "Camera.h"
#include <stdio.h>

Camera::Camera(const glm::vec3& _pos, const glm::vec3& _forward, const glm::vec3& _worldUp)
{
	FOV = glm::radians(45.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)1600 / (float)900, 0.1f, 100.0f);

	position = _pos;
	forward = _forward;
	up = _worldUp;

	yaw = -90.0f;
	pitch = 0.0f;
	roll = 0.0f;

	updateVectors();
}
 
void Camera::updateVectors()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), -forward));
	up = glm::cross(-forward, right);
}

void Camera::setFOV(const float& FOV)
{
	this->FOV = FOV;
}

void Camera::processKeyboardInput(const CAM_DIRECTION& direction, const float& speed)
{
	switch (direction)
	{
	case FORWARD:
		position += speed * forward;
		break;
	case BACK:
		position -= speed * forward;
		break;
	case LEFT:
		position -= speed * right;
		break;
	case RIGHT:
		position += speed * right;
		break;
	case UP:
		position.y += speed;
		break;
	case DOWN:
		position.y -= speed;
		break;
	}



}
void Camera::processMouseInput(const double& xOffset, const double& yOffset)
{
	yaw += xOffset * MOUSE_SENSITIVIY;
	pitch += yOffset * MOUSE_SENSITIVIY;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	updateVectors();
	//updateViewMatrix();
}

glm::mat4 Camera::genCameraMatrix()
{
	glm::mat4 matrix;
	matrix[0][0] = right.x;
	matrix[0][1] = up.x;
	matrix[0][2] = -forward.x;
	matrix[0][3] = 0.0f;

	matrix[1][0] = right.y;
	matrix[1][1] = up.y;
	matrix[1][2] = -forward.y;
	matrix[1][3] = 0.0f;

	matrix[2][0] = right.z;
	matrix[2][1] = up.z;
	matrix[2][2] = -forward.z;
	matrix[2][3] = 0.0f;

	matrix[3][0] = glm::dot(-position, right);
	matrix[3][1] = glm::dot(-position, up);
	matrix[3][2] = glm::dot(-position, -forward);
	matrix[3][3] = 1.0f;

	return matrix;
}

void Camera::updateViewMatrix()
{
	view = glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
}

void Camera::debug_dump()
{
	printf("Pos:\t\t%f , %f , %f\n", position.x, position.y, position.z);
	printf("Forward:\t%f , %f , %f\n", forward.x, forward.y, forward.z);
	printf("Right:\t\t%f , %f , %f\n", right.x, right.y, right.z);
	printf("Up:\t\t%f , %f , %f\n", up.x, up.y, up.z);

}
