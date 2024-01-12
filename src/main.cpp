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

#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Texture2D.h"
#include "renderer/Camera.h"
#include "renderer/Mesh.h"
#include "renderer/Model.h"

#include "helpers/vertices.h"
#include "helpers/lights.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void printMat(const glm::mat4& mat, const int& size);
glm::mat4 genNormalTransform(const glm::mat4& transform);



const glm::mat4 Identity(1.0f);
const float MOVE_SPEED = 10;
const int WIDTH = 1600;
const int HEIGHT = 900;

float deltaTime = 0;
float lastFrame = 0;
float lastMouseX = WIDTH / 2;
float lastMouseY = HEIGHT / 2;

namespace Space
{
	int a;
	std::string s;
}

Camera fpsCam(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));


bool firstMouse = true;


int main()
{

	fpsCam.debug_dump();

	std::cout << std::endl << sizeof(Texture2D) << std::endl;
	glfwInit();


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//printf("%d", sizeof(Shader));

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "rank1 KR openGL window", NULL, NULL);
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

	glClearColor(4.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//cam.position = glm::vec3(0.0f, 0.0f, 5.0f);
	//cam.upVec = glm::vec3(0.0f, 1.0f, 0.0f);
	//cam.rightVec = glm::vec3(1.0f, 0.0f, 0.0f);
	//cam.forwardVec = glm::vec3(0.0f, 0.0f, -1.0f);




	glEnable(GL_DEPTH_TEST);

	unsigned int vbo, vao, lightVao;


	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &lightVao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_pos_normal_tex), cube_pos_normal_tex, GL_STATIC_DRAW);

	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(lightVao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	Model backpackModel("NewSponza_Main_glTF_002.gltf");
	Model actualPackModel("backpack.obj");

	Texture2D diffuse("container2.png", 0);
	Texture2D specular("container2_specular.png", 1);
		

	Shader lampShader("basic_vert.glsl", "light_source_frag.glsl");
	Shader phongShader("vert_normals.glsl", "phong_frag.glsl");
	Shader packShader("vert_normals.glsl", "phong_color_frag.glsl");

	glActiveTexture(GL_TEXTURE0);
	diffuse.bind();
	phongShader.use();
	phongShader.setInt("material.diffuse", 0);
	glActiveTexture(GL_TEXTURE1);
	specular.bind();
	phongShader.setInt("material.specular", 1);

	

	glm::mat4 cameraTransform;


	glm::mat4 worldTransform = glm::mat4(1.0f);
	worldTransform = glm::translate(worldTransform, glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 cube2WorldTransform = glm::mat4(1.0f);
	cube2WorldTransform = glm::translate(cube2WorldTransform, glm::vec3(1.0f, 0.0f, -2.0f));
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 200.0f);
	//glm::mat4 glmCam = glm::lookAt(cam.position, glm::vec3(0.0f, 0.0f, 0.0f), cam.upVec);


	glm::vec4 lightPos(0.0f, 0.3f, 3.0f, 1.0f);

	//
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;

		lastFrame = currentTime;
		float fps = 1.0f / deltaTime;

		int f = (int)fps;
		std::ostringstream ss;
		ss << "fps " << f;
		glfwSetWindowTitle(window, ss.str().c_str());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processInput(window);

		//updateCameraTransform(cameraTransform);
		fpsCam.genCameraMatrix(cameraTransform);





		// cube
		glm::vec3 cubePos(0.0f, 0.0f, -3.0f);
		glm::mat4 cubeTransform(1.0f);
		glm::mat3 normalTransform;
		cubeTransform = glm::translate(cubeTransform, cubePos);

		//normalTransform = glm::transpose(glm::inverse(cameraTransform * cubeTransform));
		//more expensive way (lol):
		normalTransform = genNormalTransform(cameraTransform * cubeTransform);


		//define some lights

		//yellow light:
		dirLight yellowDirLight
		{
			glm::vec3(1.0f, -1.0f, -1.0f),
			glm::vec3(0.1f),
			glm::vec3(1.5f, 1.0f, 0.0f),
			glm::vec3(1.0f),
		};

		dirLight redDirLight
		{
			glm::vec3(-1.0f, 0.5f, -1.0f),
			glm::vec3(0.4f),
			glm::vec3(0.7f, 0.0f, 0.0f),
			glm::vec3(0.1f),
		};

		pointLight bluePointLight
		{
			glm::vec3(1.0f, 0.5f, 1.0f),
			glm::vec3(0.3f),
			glm::vec3(0.0f, 0.0f, 10.0f),
			glm::vec3(1.0f),
			1.0f,
			0.009f,
			0.0032f
		};

		spotLight flashLight
		{
			glm::vec3(0, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec3(0.0f),
			glm::vec3(1.0f),
			glm::vec3(1.0f),
			glm::cos(glm::radians(5.0f)),
			glm::cos(glm::radians(10.0f)),
			1.0f,
			0.9f,
			0.32f
		};

		glm::mat4 sinTransform(1.0f);
		sinTransform = glm::translate(sinTransform, glm::vec3(0.0f, 0.0f, -1.0f * sin(5.0f * glfwGetTime())));
		bluePointLight.lightPos = sinTransform * glm::vec4(bluePointLight.lightPos, 1.0f);

		//render light sources

		lampShader.use();
		lampShader.setMat4("view", cameraTransform);
		lampShader.setMat4("proj", proj);

		//set color and world matrix for each light source.
		glm::mat4 scaleI = glm::scale(Identity, glm::vec3(0.3f));
		glm::mat4 lightTransform = glm::translate(scaleI, -20.0f * yellowDirLight.lightDir);

		lampShader.setMat4("world", lightTransform);
		lampShader.setVec3("lightColor", yellowDirLight.diffuse);
		glBindVertexArray(lightVao);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		lightTransform = glm::translate(scaleI, -20.0f * redDirLight.lightDir);
		lampShader.setMat4("world", lightTransform);
		lampShader.setVec3("lightColor", redDirLight.diffuse);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		lightTransform = glm::translate(scaleI, bluePointLight.lightPos);
		lampShader.setMat4("world", lightTransform);
		lampShader.setVec3("lightColor", bluePointLight.diffuse);
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		// change light source vectors to view space
		yellowDirLight.lightDir = cameraTransform * glm::vec4(yellowDirLight.lightDir, 0.0f);
		redDirLight.lightDir = cameraTransform * glm::vec4(redDirLight.lightDir, 0.0f);
		bluePointLight.lightPos = cameraTransform * glm::vec4(bluePointLight.lightPos, 1.0f);

		phongShader.use();
		phongShader.setDirLight("directional_lights[0]", yellowDirLight);
		phongShader.setDirLight("directional_lights[1]", redDirLight);
		phongShader.setSpotLight("spot_lights[0]", flashLight);
		phongShader.setPointLight("point_lights[0]", bluePointLight);

		phongShader.setInt("NUM_DIR_LIGHTS", 2);
		phongShader.setInt("NUM_POINT_LIGHTS", 1);
		phongShader.setInt("NUM_SPOT_LIGHTS", 1);

		phongShader.setFloat("material.shininess", 16.0f);
		phongShader.setMat4("view", cameraTransform);
		phongShader.setMat4("proj", proj);
		phongShader.setMat4("normalTransform", normalTransform);



		//render objects



		packShader.use();
		packShader.setMat4("world", cubeTransform);
		packShader.setMat4("view", cameraTransform);
		packShader.setMat4("proj", proj);
		packShader.setMat4("normalTransform", normalTransform);

		packShader.setVec3("material.diffuse", glm::vec3(0.1f, 1.0f, 0.1f));
		packShader.setVec3("material.specular", glm::vec3(1.0f));
		packShader.setFloat("material.shininess", 32.0f);



		packShader.setDirLight("directional_lights[0]", yellowDirLight);
		packShader.setDirLight("directional_lights[1]", redDirLight);

		packShader.setInt("NUM_DIR_LIGHTS", 2);

		glm::mat4 tw = cubeTransform;

		scaleI = glm::scale(scaleI, glm::vec3(5.0f));
		cubeTransform = glm::translate(scaleI, glm::vec3(0.0f, -2.5f, -3.0f));

		glBindVertexArray(vao);

		tw = glm::translate(cubeTransform, glm::vec3(0.0f, 0.0f, -15.0f));
		normalTransform = genNormalTransform(cameraTransform * cubeTransform);
		packShader.setMat4("world", tw);
		packShader.setMat4("normalTransform", normalTransform);
		Renderer::drawSolidModel(backpackModel, packShader);

		tw = glm::translate(tw, glm::vec3(10.0f, 0.0f, 10.0f));
		packShader.setMat4("world", tw);
		Renderer::drawSolidModel(actualPackModel, packShader);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}







	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	float speed = (float)MOVE_SPEED * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		fpsCam.processKeyboardInput(CAM_DIRECTION::FORWARD, speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		fpsCam.processKeyboardInput(CAM_DIRECTION::BACK, speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		fpsCam.processKeyboardInput(CAM_DIRECTION::LEFT, speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		fpsCam.processKeyboardInput(CAM_DIRECTION::RIGHT, speed);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		fpsCam.processKeyboardInput(CAM_DIRECTION::UP, speed);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		fpsCam.processKeyboardInput(CAM_DIRECTION::DOWN, speed);


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

	fpsCam.processMouseInput(x_offset, y_offset);
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