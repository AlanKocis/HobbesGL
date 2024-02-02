#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "renderer/Scene.h"


class GUI
{
public:
	bool showGui;
	bool disabledGui;
	Scene* target_scene;

	GUI();

	void init(Scene* scene);

	void renderGUI();

	void toggleDisableGUI();

	void disableGUI();

	void enableGUI();

	void update();
	void showLightsDebug();

	~GUI();
};