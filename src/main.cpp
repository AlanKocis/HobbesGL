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
#include <tuple>

#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Texture2D.h"
#include "renderer/Camera.h"
#include "renderer/Mesh.h"
#include "renderer/Model.h"
#include "renderer/Scene.h"
#include "renderer/Light.h"

#include "helpers/vertices.h"
#include "helpers/GUI.h"
//#include "renderer/EngineAssets.h"


/*
@armaanc.684
1 year ago (edited)
For the latest ImGui version, the files Cherno copied are located in the backends folder of imgui, also, to use glad with imgui, there will be a #include "imgui_impl_opengl3_loader.h", you need to change the entire block, including the #define and #endif to:
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <glad/glad.h>
#endif
There is no need to have the imgui_impl_opengl3_loader.h file as we are already using glad.

*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void printMat(const glm::mat4& mat, const int& size);
glm::mat4 genNormalTransform(const glm::mat4& transform);

GUI gui;

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

Camera FpsCam(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

bool firstMouse = true;




int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(4.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//cam.position = glm::vec3(0.0f, 0.0f, 5.0f);
	//cam.upVec = glm::vec3(0.0f, 1.0f, 0.0f);
	//cam.rightVec = glm::vec3(1.0f, 0.0f, 0.0f);
	//cam.forwardVec = glm::vec3(0.0f, 0.0f, -1.0f);

	glEnable(GL_DEPTH_TEST);




	Scene editor_scene;
	gui.init(&editor_scene);
	editor_scene.m_currentCamera = &FpsCam;

	Model backpackModel("Articulated_Worm.obj");
	//Mesh floorMesh(CUBE_VERT_VECTOR, Texture2D("red.jpg", DIFFUSE));
	//Object backpack(&DefinedShaders::phong_lighting, &backpackModel);
	//Object floor(&shaderDiffOnly, &floorMesh);

	Shader shaderDiffOnly("vert_normals.glsl", "phong_color_frag.glsl");
	Shader shader("vert_normals.glsl", "phong_frag.glsl");



	editor_scene.createAddObject(&shader, &backpackModel);
	editor_scene.createAddObject(&shader, Mesh::genCreateQuadMesh(1, 1, 1));

	for (Mesh* m : editor_scene.m_objects[0]->m_model->m_meshes)
	{
		m->m_textures.push_back(Texture2D("DEFAULT_TEX_WHITE.jpg", DIFFUSE));
	}
	

	shaderDiffOnly.use();
	shaderDiffOnly.setVec3("material.diffuse", glm::vec3(0.0f, 1.0f, 0.0f));

	shaderDiffOnly.setVec3("material.specular", glm::vec3(10.0f));
	shaderDiffOnly.setFloat("material.shininess", 16.0f);



	//PointLight pointlight1(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(2.0f), glm::vec3(0.0f, 1.0f, -5.0f));
	//editor_scene.addLight(&pointlight1);
	editor_scene.m_objects[0]->m_transform.pos = glm::vec3(0.0f, -1.0f, -20.0f);
	editor_scene.m_objects[0]->m_transform.scale = glm::vec3(0.1f);

	//editor_scene.m_objects[1]->m_transform.scale = glm::vec3(100.0f, 0.2f, 100.0f);
	//editor_scene.m_objects[1]->m_transform.pos = glm::vec3(0.0f, -20.0f, 0.0f);

	while (!glfwWindowShouldClose(window))
	{
		glm::vec4 dir = FpsCam.genCameraMatrix() * glm::vec4(1);
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;

		lastFrame = currentTime;
		float fps = 1.0f / deltaTime;

		int f = (int)fps;
		std::ostringstream ss;
		ss << "fps " << f;
		glfwSetWindowTitle(window, ss.str().c_str());

		glm::vec3 skyColor(76, 64, 142);
		skyColor = glm::normalize(skyColor);
		//glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);		
		
		glClearColor(0.0F, 0.0F, 0.0f, 1.0f);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);


		//put shit here



		editor_scene.updateScene();

		gui.update();







		Renderer::drawScene(&editor_scene);		
		gui.renderGUI();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}


	editor_scene.freeAllLights();
	editor_scene.freeAllObjects();



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
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		gui.enableGUI();
		firstMouse = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		FpsCam.processKeyboardInput(CAM_DIRECTION::FORWARD, speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		FpsCam.processKeyboardInput(CAM_DIRECTION::BACK, speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		FpsCam.processKeyboardInput(CAM_DIRECTION::LEFT, speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		FpsCam.processKeyboardInput(CAM_DIRECTION::RIGHT, speed);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		FpsCam.processKeyboardInput(CAM_DIRECTION::UP, speed);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		FpsCam.processKeyboardInput(CAM_DIRECTION::DOWN, speed);


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

	if(gui.disabledGui)
		FpsCam.processMouseInput(x_offset, y_offset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			gui.disableGUI();

		}
		
	}
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

struct ArenaPool
{
	unsigned char* buff;
	size_t offset;
	size_t buffer_size;
};

void* arena_alloc(struct ArenaPool* arena, size_t size)
{
	if (arena->offset + size <= arena->buffer_size)
	{
		void* ptr = (arena->buff + arena->offset);
		arena->offset += size;
		memset(ptr, 0, size);
		return ptr;
	}
	else
		return NULL;
}