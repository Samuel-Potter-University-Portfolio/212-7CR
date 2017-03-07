#pragma once
#include <map>
#include <string>
#include "Shader.h"


struct ShaderData
{
	Shader* shader;
};


class ShaderLoader
{
private:
	std::map<std::string, ShaderData> shader_pool;

public:

	void RegisterShader(const std::string name, Shader* shader);
	Shader* operator[](const std::string key);

	void CleanUp();
};
