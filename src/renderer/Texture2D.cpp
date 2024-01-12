#include <glad/glad.h>
#include "renderer/Texture2D.h"
#include <stb_image.h>
#include <helpers/RootDir.h>
#include <stdio.h>
#include "renderer/Renderer.h"

Texture2D::Texture2D(std::string filePath, int type)
{
	mType = type;
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int _width, _height, _nrChannels;

	std::string path = ROOT_DIR "res/textures/" + filePath;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &_nrChannels, 0);
	if (data)
	{
		if (_nrChannels > 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			printf("called glTexImage2D on %s with GL_RGBA\n", filePath.c_str());
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			mPath = filePath;
			printf("called glTexImage2D on %s with GL_RGB\n", filePath.c_str());
		}
	}
	else
	{
		printf("Failed to load texture: %s\n", filePath.c_str());
	}


	stbi_image_free(data);

}

void Texture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, mID);
}