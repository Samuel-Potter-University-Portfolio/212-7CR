#include "ModelLoader.h"
#include "Logger.h"
#include "OBJLoader.h"
#include <algorithm>


void ModelLoader::RegisterModel(const std::string name, const ModelMesh& mesh) 
{
	//Ensure key is stored in lower case to prevent duplicates
	std::string key = name;
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (model_pool.find(key) != model_pool.end())
	{
		LOG(Warning, "Failed attempt to re-register model '%s'", key.c_str());
		return;
	}

	Model* model = CreateModel(mesh);
	ModelData meta_data;

	meta_data.model = model;
	meta_data.imported = false;

	model_pool[key] = meta_data;
	LOG(Log, "Registered model '%s'", key.c_str());
}

Model* ModelLoader::operator[](const std::string name)
{
	//Ensure key is stored in lower case to prevent duplicates
	std::string key = name;
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);

	if (model_pool.find(key) == model_pool.end())
	{
		//If can't find and path ends in '.obj', attempt to load file
		if (name.length() >= 4 && name.compare(name.length() - 4, 4, ".obj") == 0)
		{
			ModelData meta_data;
			meta_data.model = CreateModel(OBJLoader::Load(name, import_scale));
			meta_data.imported = true;
			meta_data.file_path = name;
			model_pool[key] = meta_data;
			LOG(Log, "Registered model '%s'", key.c_str());

			return model_pool[key].model;
		}
		else
			return nullptr;
	}
	return model_pool[key].model;
}

void ModelLoader::CleanUp() 
{
	LOG(Log, "Cleaning up models:");

	GLuint current_vao;
	for (auto it = model_pool.begin(); it != model_pool.end(); ++it)
	{
		Model* model = it->second.model;
		current_vao = model->GetVAO();
		glDeleteVertexArrays(1, &current_vao);
		delete model;

		LOG(Log, "\t-Unloaded model '%s'", it->first.c_str());
	}
}

Model* ModelLoader::CreateModel(const ModelMesh& mesh) 
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	StoreIndices(mesh.GetRawIndices());
	StoreData(0, 3, mesh.GetRawVertices(), GL_STATIC_DRAW);
	StoreData(1, 2, mesh.GetRawUVs(), GL_STATIC_DRAW);
	StoreData(2, 3, mesh.GetRawNormals(), GL_STATIC_DRAW);
	glBindVertexArray(0);

	Model* model = new Model(vao, mesh.GetRawIndices().size());
	return model;
}

GLuint ModelLoader::StoreIndices(const std::vector<int>& indices) 
{
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);

	if (indices.size())
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
	else
	{
		std::vector<int> new_data{ 0,0,0 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, new_data.size() * sizeof(int), &new_data[0], GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

GLuint ModelLoader::StoreData(const GLuint attribute_num, const int data_size, const std::vector<float>& data, const GLenum vbo_type) 
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	if(data.size())
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], vbo_type);
	else
	{
		std::vector<float> new_data { 0,0,0 };
		glBufferData(GL_ARRAY_BUFFER, new_data.size() * sizeof(float), &new_data[0], vbo_type);
	}

	glEnableVertexAttribArray(attribute_num);
	glVertexAttribPointer(attribute_num, data_size, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}
