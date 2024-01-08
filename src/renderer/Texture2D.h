#pragma once
#include <stb_image.h>
#include <string>

class Texture2D
{
public:
	unsigned int ID;
	unsigned int width;
	unsigned int height;
	unsigned int nrChannels;

	Texture2D(std::string filePath);
	void bind();


};