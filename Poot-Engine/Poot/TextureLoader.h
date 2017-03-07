#pragma once
#include "API.h"
#include <string>
#include <map>
#include <GL/glew.h>

struct TextureData 
{
	GLuint texture_id;
	bool imported;
	std::string file_path;
};


class POOT_API TextureLoader
{
private:
	std::map<std::string, TextureData> texture_pool;
	std::map<std::string, TextureData> cube_map_pool;


public:
	GLuint operator[](std::string name);
	GLuint GetCubeMap(std::string name);

	void CleanUp();
};

