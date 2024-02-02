#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 world;
uniform mat4 normalTransform;

void main()
{

	FragPos = vec3(view * world * vec4(aPos, 1.0));
	//Normal = vec3(normalTransform * vec4(aNormal, 0.0));
	Normal = vec3(transpose(inverse(view * world)) * vec4(aNormal, 0.0f));

	TexCoord = aTexCoord;
	gl_Position = proj * vec4(FragPos, 1.0);
}