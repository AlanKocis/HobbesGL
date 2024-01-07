#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

#include "renderer/Shader.h"
#include "renderer/Camera.h"
#include "renderer/Renderer.h"
#include "helpers/vertices.h"
#include "helpers/lights.h"
#include <helpers/RootDir.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void printMat(const glm::mat4& mat, const int& size);
glm::mat4 genNormalTransform(const glm::mat4& transform);

const glm::mat4 Identity(1.0f);


struct
{
	float pitch;
	float yaw;
	float roll;
} typedef eulers;

const float MOVE_SPEED = 10;
const int WIDTH = 1600;
const int HEIGHT = 900;

float deltaTime = 0;
float lastFrame = 0;
float lastMouseX = WIDTH / 2;
float lastMouseY = HEIGHT / 2;

Camera fpsCam(glm::vec3(0, 0, 3), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
eulers mouseAngles{ 0.0f, -90.0f, 0 };

bool firstMouse = true;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hobbes-1.1", NULL, NULL);
	if (!window)
	{
		std::cout << "window failed\n";
		return -1;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD failed\n";
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	// set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);




	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_pos_normal_tex), cube_pos_normal_tex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	spotLight flashLight
	{
		glm::vec3(0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.3f),
		glm::vec3(1.0f),
		glm::vec3(1.5f),
		glm::radians(5.0f),
		glm::radians(14.0f),
		1.0f,
		0.09f,
		0.032f
	};

	Shader* phong = new Shader("basic_vert.glsl", "basic_frag.glsl");
	phong->use();
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	phong->setMat4("proj", proj);

	//
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		float fps = 1000 / deltaTime;
		int f = (int)fps;
		std::ostringstream ss;
		ss << "fps " << f;
		glfwSetWindowTitle(window, ss.str().c_str());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);

	

		glm::vec3 cubePos(0.0f, 0.0f, -3.0f);
		glm::mat4 cubeTransform = glm::translate(Identity, cubePos);

		fpsCam.updateCameraTransform();
		phong->setMat4("view", fpsCam.c_viewMat);
		phong->setMat4("world", cubeTransform);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
		
		





		glfwSwapBuffers(window);
		glfwPollEvents();

	}



	delete phong;

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}




void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	float speed = (float)MOVE_SPEED * deltaTime;
	fpsCam.processInput(window, speed);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float x_offset = xpos - lastMouseX;
	float y_offset = lastMouseY - ypos;	//screen coordinates have reversed y axis

	lastMouseX = xpos;
	lastMouseY = ypos;

	float sensitivity = 0.1;

	x_offset *= sensitivity;
	y_offset *= sensitivity;

	mouseAngles.yaw += x_offset;
	mouseAngles.pitch += y_offset;

	if (mouseAngles.pitch > 89.0f)
		mouseAngles.pitch = 89.0f;
	if (mouseAngles.pitch < -89.0f)
		mouseAngles.pitch = -89.0f;

	glm::vec3 camFront;
	camFront.x = cos(glm::radians(mouseAngles.yaw)) * cos(glm::radians(mouseAngles.pitch));
	camFront.y = sin(glm::radians(mouseAngles.pitch));
	camFront.z = sin(glm::radians(mouseAngles.yaw)) * cos(glm::radians(mouseAngles.pitch));

	fpsCam.setForward(glm::normalize(camFront));
}

void printMat(const glm::mat4& mat, const int& size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (mat[j][i] >= 0)
				std::cout << ' ';
			std::cout << std::setprecision(2) << mat[j][i] << '\t';
		}
		std::cout << '\n';
	}
}

glm::mat4 genNormalTransform(const glm::mat4& transform)
{
	glm::mat4 normTransf;

	//std::cout << "\n\n\n";

	//1. calculate determinants for each Minor matrix in transform
	//			loop through size and continue on i and j
	// 
	//2. C = det(M) * (-1)^ (i+j+2)
	//set C to the value of normTransf[j][i]
	glm::mat3 minor;

	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			//clear minor matrix
			minor = glm::mat3(0.0f);

			int mRow = 0;
			int mCol = 0;

			for (int innerRow = 0; innerRow < 4; innerRow++)
			{
				if (innerRow == row)
					continue;
				for (int innerCol = 0; innerCol < 4; innerCol++)
				{
					if (innerCol == col)
						continue;

					minor[mCol][mRow] = transform[innerCol][innerRow];
					mCol++;
				}
				mRow++;
				mCol = 0;
			}

			//find det
			std::vector<float> m2x2;
			std::vector<float> determinants;

			for (int i = 0; i < 3; i++)
			{
				m2x2.clear();

				for (int innerRow = 1; innerRow < 3; innerRow++)
				{
					for (int innerCol = 0; innerCol < 3; innerCol++)
					{
						if (innerCol == i)
							continue;

						m2x2.push_back(minor[innerCol][innerRow]);
					}
					mRow++;
					mCol = 0;
				}

				float det = (m2x2[0] * m2x2[3]) - (m2x2[1] * m2x2[2]);
				det = det * (float)std::pow(-1, i + 2);
				det = det * minor[i][0];

				determinants.push_back(det);
			}


			//3x3 = det + det + det
			normTransf[col][row] = determinants[0] + determinants[1] + determinants[2];
			normTransf[col][row] *= (float)std::pow(-1, row + col + 2);
			determinants.clear();

		}

	}
	int j = 1;
	return normTransf;
}