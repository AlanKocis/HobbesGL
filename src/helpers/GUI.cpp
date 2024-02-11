#include "helpers/GUI.h"
#include <vector>

void setEditVec(float* v, glm::vec3* vec)
{
	v[0] = vec->x;
	v[1] = vec->y;
	v[2] = vec->z;
}



void guiSetLightVec(Light* light, LightType type)
{

}


GUI::GUI()
{
	showGui = true;
	disabledGui = true;
	target_scene = nullptr;
}

void GUI::init(Scene* scene)
{

	target_scene = scene;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::renderGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

}

void GUI::toggleDisableGUI()
{
	disabledGui = !disabledGui;
}

void GUI::disableGUI()
{
	disabledGui = true;
}

void GUI::enableGUI()
{
	disabledGui = false;
}


void GUI::update()
{
	// Helper to wire demo markers located in code to an interactive browser
	typedef void (*ImGuiDemoMarkerCallback)(const char* file, int line, const char* section, void* user_data);
	extern ImGuiDemoMarkerCallback      GImGuiDemoMarkerCallback;
	extern void* GImGuiDemoMarkerCallbackUserData;
	ImGuiDemoMarkerCallback             GImGuiDemoMarkerCallback = NULL;
	void* GImGuiDemoMarkerCallbackUserData = NULL;
	#define IMGUI_DEMO_MARKER(section)  do { if (GImGuiDemoMarkerCallback != NULL) GImGuiDemoMarkerCallback(__FILE__, __LINE__, section, GImGuiDemoMarkerCallbackUserData); } while (0)




	ImGui::BeginDisabled();
	if (!disabledGui)
		ImGui::EndDisabled();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	static bool showAddLights = false;
	ImGui::Begin("hobbes debug");

	IMGUI_DEMO_MARKER("Lights");
	if (ImGui::CollapsingHeader("Lights"))
	{
		if (ImGui::Button("add light source"))
		{
			showAddLights = true;
		}
		if (showAddLights)
		{
			if (ImGui::Button("directional"))
			{
				target_scene->createAddLight(DIR);
				showAddLights = false;
			}
			if (ImGui::Button("point"))
			{
				target_scene->createAddLight(POINT);
				showAddLights = false;
			}
			if (ImGui::Button("spot"))
			{
				target_scene->createAddLight(SPOT);
				showAddLights = false;
			}

		}

		//showLightsDebug();

		
		static void* editLightVecPtr = nullptr;
		static bool showEditVec = false;
		static float vecDigits[3]{ 0.0f, 0.0f, 0.0f };

		if (ImGui::TreeNode("scene lights:"))
		{


			unsigned int lightIndex = 0;
			for (Light* light : target_scene->m_lights)
			{
				LightType t = light->getType();
				char* s = "";
				switch (t)
				{
				case DIR:
					s = "directional light ";
					break;
				case SPOT:
					s = "spot light ";
					break;
				case POINT:
					s = "point light ";
					break;
				}
				static bool a;
				void* ptr = (void*)light;



				if (ImGui::TreeNode((void*)(light), s))
				{
					//ImGui::Text("pos: %f, %f, %f", light->);

					switch (t)
					{
					case DIR:
						ImGui::Text("Pos: %.3f, %.3f, %.3f", ((DirLight*)ptr)->m_lightDirection.x, ((DirLight*)ptr)->m_lightDirection.y, ((DirLight*)ptr)->m_lightDirection.z);
						ImGui::SameLine();
						if (ImGui::Button("Edit#1"))
						{
							editLightVecPtr = &((DirLight*)ptr)->m_lightDirection;
							showEditVec = true;
							setEditVec(vecDigits, (glm::vec3*)editLightVecPtr);
						}


						break;
					case SPOT:
						ImGui::Text("Dir: %.3f, %.3f, %.3f", ((SpotLight*)ptr)->m_lightDirection.x, ((SpotLight*)ptr)->m_lightDirection.y, ((SpotLight*)ptr)->m_lightDirection.z);
						ImGui::SameLine();
						if (ImGui::Button("Edit##2"))
						{
							printf("bb\n");
							editLightVecPtr = &((SpotLight*)ptr)->m_lightDirection;
							showEditVec = true;
							setEditVec(vecDigits, (glm::vec3*)editLightVecPtr);
						}
						ImGui::Text("Pos: %.3f, %.3f, %.3f", ((SpotLight*)ptr)->m_lightPosition.x, ((SpotLight*)ptr)->m_lightPosition.y, ((SpotLight*)ptr)->m_lightPosition.z);
						ImGui::SameLine();
						if (ImGui::Button("Edit##3"))
						{
							printf("aa\n");
							editLightVecPtr = &((SpotLight*)ptr)->m_lightPosition;
							showEditVec = true;
							setEditVec(vecDigits, (glm::vec3*)editLightVecPtr);

						}
						break;
					case POINT:
						ImGui::Text("Pos: %.3f, %.3f, %.3f", ((PointLight*)ptr)->m_lightPosition.x, ((PointLight*)ptr)->m_lightPosition.y, ((PointLight*)ptr)->m_lightPosition.z);
						ImGui::SameLine();
						if (ImGui::Button("Edit##4"))
						{
							editLightVecPtr = &((PointLight*)ptr)->m_lightPosition;
							showEditVec = true;
							setEditVec(vecDigits, (glm::vec3*)editLightVecPtr);
						}
						break;
					}

					ImGui::Text("Diffuse: %.3f, %.3f, %.3f", ((Light*)ptr)->m_diffuse.x, ((Light*)ptr)->m_diffuse.y, ((Light*)ptr)->m_diffuse.z);
					ImGui::SameLine();
					if (ImGui::Button("Edit##5"))
					{
						editLightVecPtr = &((Light*)ptr)->m_diffuse;
						showEditVec = true;
						setEditVec(vecDigits, (glm::vec3*)editLightVecPtr);
					}
					ImGui::Text("Ambient: %.3f, %.3f, %.3f", ((Light*)ptr)->m_ambient.x, ((Light*)ptr)->m_ambient.y, ((Light*)ptr)->m_ambient.z);
					ImGui::SameLine();
					if (ImGui::Button("Edit##6"))
					{
						editLightVecPtr = &((Light*)ptr)->m_ambient;
						showEditVec = true;
						setEditVec(vecDigits, (glm::vec3*)editLightVecPtr);

					}
					ImGui::Text("Specular: %.3f, %.3f, %.3f", ((Light*)ptr)->m_specular.x, ((Light*)ptr)->m_specular.y, ((Light*)ptr)->m_specular.z);
					ImGui::SameLine();
					if (ImGui::Button("Edit##7"))
					{
						editLightVecPtr = &((Light*)ptr)->m_specular;
						showEditVec = true;
						setEditVec(vecDigits, (glm::vec3*)editLightVecPtr);
					}

					if (ImGui::Button("delete light"))
					{
						target_scene->setDeleteLightIndex(lightIndex);
					}
						




					ImGui::TreePop();
				}


				
				lightIndex++;
			}

			ImGui::TreePop();
			
		}


		if (showEditVec)
		{
			glm::vec4* editVector = (glm::vec4*)editLightVecPtr;



			printf("edit true\n");
			ImGui::InputFloat3("input float3", vecDigits);

			if (ImGui::Button("set"))
			{
				editVector->x = vecDigits[0];
				editVector->y = vecDigits[1];
				editVector->z = vecDigits[2];


				showEditVec = false;
				vecDigits[0] = 0.0f;
				vecDigits[1] = 0.0f;
				vecDigits[2] = 0.0f;

			}

		}






	}


	if (ImGui::CollapsingHeader("Objects"))
	{


	}


		

	ImGui::End();

	//ImGui::ShowDemoWindow(&showGui);
}


void GUI::showLightsDebug()
{



}

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

