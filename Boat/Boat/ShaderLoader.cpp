#include "ShaderLoader.h"
#include "Logger.h"
#include <algorithm>


void ShaderLoader::RegisterShader(const std::string name, Shader* shader)
{
	//Ensure key is stored in lower case to prevent duplicates
	std::string key = name;
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (shader_pool.find(key) != shader_pool.end())
	{
		LOG(Warning, "Failed attempt to re-register shader '%s'", key.c_str());
		delete shader;
		return;
	}

	ShaderData meta_data;
	meta_data.shader = shader;
	shader->Load();

	shader_pool[key] = meta_data;
	LOG(Log, "Registered shader '%s'", key.c_str());
}

Shader* ShaderLoader::operator[](const std::string key)
{
	if (shader_pool.find(key) == shader_pool.end())
		return nullptr;
	return shader_pool[key].shader;
}

void ShaderLoader::CleanUp()
{
	LOG(Log, "Cleaning up shaders:");

	GLuint current_vao;
	for (auto it = shader_pool.begin(); it != shader_pool.end(); ++it)
	{
		Shader* shader = it->second.shader;
		shader->CleanUp();
		delete shader;

		LOG(Log, "\t-Unloaded shader '%s'", it->first.c_str());
	}
}