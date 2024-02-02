#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CAM_DIRECTION
{
	FORWARD,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
	glm::vec3 right;
	glm::mat4 projection;
	glm::mat4 view;
	float FOV;
	float pitch;
	float yaw;
	float roll;

	float MOUSE_SENSITIVIY = 0.1f;

	Camera(const glm::vec3& _pos, const glm::vec3& _forward, const glm::vec3& _worldUp);
	void updateVectors();
	void setFOV(const float& FOV);
	void processKeyboardInput(const CAM_DIRECTION& direction, const float& speed);
	void processMouseInput(const double& xOffset, const double& yOffset);
	glm::mat4 genCameraMatrix();
	glm::mat4 genProjectionMatrix() { return projection; }
	void updateViewMatrix();
	void debug_dump();

};