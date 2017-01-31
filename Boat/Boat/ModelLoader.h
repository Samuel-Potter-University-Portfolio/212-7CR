#pragma once
#include <vector>
#include <GL/glew.h>

#include "Mesh.h"
#include "Model.h"

class ModelLoader
{
private:
	std::vector<Model*> models;

	static GLuint StoreIndices(const std::vector<int> indices);
	static GLuint StoreData(const GLuint attribute_num, const int data_size, std::vector<float>& data, const GLenum vbo_type);

public:
	Model* CreateModel(const Mesh& mesh);

	void CleanUp();
};

