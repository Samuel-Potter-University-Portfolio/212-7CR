#pragma once
#include <string>
#include <map>
#include <GL/glew.h>

struct TextureData 
{
	GLuint texture_id;
	bool imported;
	std::string file_path;
};


class TextureLoader
{
private:
	std::map<std::string, TextureData> texture_pool;


public:
	GLuint operator[](std::string name);

	void CleanUp();
};

