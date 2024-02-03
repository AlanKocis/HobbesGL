#pragma once
#include "renderer/Shader.h"


namespace DefinedShaders
{
	const Shader phong_lighting_texture("vert_normals.glsl", "phong_frag.glsl");
	const Shader phong_lighting_solid_color("vert_normals.glsl", "phong_color_frag");

};

