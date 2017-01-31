#include "ModelLoader.h"
#include "Logger.h"


void ModelLoader::CleanUp() 
{
	LOG(Log, "Cleaning up models");

	GLuint current_vao;
	for (Model* model : models)
	{
		current_vao = model->GetVAO();
		glDeleteVertexArrays(1, &current_vao);
		delete model;
	}
}

Model* ModelLoader::CreateModel(const Mesh& mesh) 
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	StoreIndices(mesh.GetRawIndices());
	StoreData(0, 3, mesh.GetRawVertices(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	Model* model = new Model(vao, mesh.GetRawIndices().size());
	models.push_back(model);
	return model;
}

GLuint ModelLoader::StoreIndices(const std::vector<int> indices) 
{
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

GLuint ModelLoader::StoreData(const GLuint attribute_num, const int data_size, std::vector<float>& data, const GLenum vbo_type) 
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], vbo_type);

	glEnableVertexAttribArray(attribute_num);
	glVertexAttribPointer(attribute_num, data_size, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}