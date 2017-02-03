#pragma once
#include <vector>
#include <map>
#include <string>
#include <GL/glew.h>

#include "Mesh.h"
#include "Model.h"


struct ModelData
{
	Model* model;
	bool imported;
	std::string file_path;
};


class ModelLoader
{
private:
	std::map<std::string, ModelData> model_pool;

	static GLuint StoreIndices(const std::vector<int> indices);
	static GLuint StoreData(const GLuint attribute_num, const int data_size, std::vector<float>& data, const GLenum vbo_type);

	Model* CreateModel(const Mesh& mesh);
public:

	void RegisterModel(const std::string name, const Mesh& mesh);
	Model* operator[](const std::string key);

	void CleanUp();
};

