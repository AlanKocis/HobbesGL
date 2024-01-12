#pragma once
#include <stb_image.h>
#include <string>

enum TEX_TYPES
{
	DIFFUSE,
	SPECULAR
};

struct Texture2D
{
public:
	unsigned int mID;
	std::string mPath;
	int mType;

	Texture2D(std::string filePath, int type);
	void bind();


};