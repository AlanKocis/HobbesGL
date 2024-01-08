#include <glad/glad.h>
#include "renderer/Texture2D.h"
#include <stb_image.h>
#include <helpers/RootDir.h>
#include <stdio.h>

Texture2D::Texture2D(std::string filePath)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int _width, _height, _nrChannels;

	std::string path = ROOT_DIR "res/textures/" + filePath;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &_nrChannels, 0);
	if (data)
	{
		if (_nrChannels > 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			printf("called glTexImage2D with GL_RGBA\n");
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			printf("called glTexImage2D with GL_RGB\n");
		}

		this->width = _width;
		this->height = _height;
		this->nrChannels = _nrChannels;
	}
	else
	{
		printf("Failed to load texture: %s", filePath.c_str());
		this->width = 0;
		this->height = 0;
		this->nrChannels = 0;
	}


	stbi_image_free(data);

}

void Texture2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}