#include "OBJLoader.h"
#include "Logger.h"

#include <vector>
#include <fstream>

void AppendVertexInfo(std::string input, std::vector<float>& values)
{
	std::string current_value;

	for (int i = 3; i < input.length(); i++)
	{
		const char c = input[i];

		if (c != ' ')
			current_value += c;
		else
		{
			values.push_back(stof(current_value));
			current_value = "";
		}
	}

	if (current_value.length())
		values.push_back(stof(current_value));
}

void AppendFaceInfo(std::string input, std::vector<int>& values)
{
	std::string current_value;

	for (int i = 2; i < input.length(); i++)
	{
		const char c = input[i];

		if (c != ' ' && c != '/')
			current_value += c;
		else
		{
			values.push_back(current_value.length() ? stoi(current_value) : 1);
			current_value = "";
		}
	}

	if (current_value.length())
		values.push_back(current_value.length() ? stoi(current_value) : 1);
}

ModelMesh Build(
	std::vector<float>& raw_vertices,
	std::vector<float>& raw_uvs,
	std::vector<float>& raw_normals,
	std::vector<int>& raw_faces)
{
	std::vector<float> vertices;
	std::vector<float> uvs;
	std::vector<float> normals;
	std::vector<int> indices;

	//Give null entries, if empty
	if (raw_vertices.size() == 0)
	{
		raw_vertices.push_back(0);
		raw_vertices.push_back(0);
		raw_vertices.push_back(0);
	}
	if (raw_uvs.size() == 0)
	{
		raw_uvs.push_back(0);
		raw_uvs.push_back(0);
	}
	if (raw_normals.size() == 0)
	{
		raw_normals.push_back(0);
		raw_normals.push_back(0);
	}

	for (int n = 0; n < raw_faces.size(); n+=3)
	{
		const int iV = raw_faces[n] - 1;
		const int iT = raw_faces[n + 1] - 1;
		const int iN = raw_faces[n + 2] - 1;

		const float vertex[3]
		{
			raw_vertices[iV * 3],
			raw_vertices[iV * 3 + 1],
			raw_vertices[iV * 3 + 2],
		};

		const float normal[3]
		{
			raw_normals[iN * 3],
			raw_normals[iN * 3 + 1],
			raw_normals[iN * 3 + 2],
		};

		const float uv[2]
		{
			raw_uvs[iT * 2],
			raw_uvs[iT * 2 + 1],
		};

		//Optimize mesh storage (Don't store duplicate entries)
		//Check if grouping already exists
		bool found = false;

		for (int i = 0; i < vertices.size() / 3; i++)
		{
			const bool vertex_equal =
				vertex[0] == vertices[i * 3] &&
				vertex[1] == vertices[i * 3 + 1] &&
				vertex[2] == vertices[i * 3 + 2];

			const bool normal_equal =
				normal[0] == normals[i * 3] &&
				normal[1] == normals[i * 3 + 1] &&
				normal[2] == normals[i * 3 + 2];

			const bool uvs_equal =
				uv[0] == uvs[i * 2] &&
				uv[1] == uvs[i * 2 + 1];

			//Found matching grouping
			if (vertex_equal && normal_equal && uvs_equal)
			{
				found = true;
				indices.push_back(i);
				break;
			}
		}

		//Create new grouping
		if (!found)
		{
			vertices.push_back(vertex[0]);
			vertices.push_back(vertex[1]);
			vertices.push_back(vertex[2]);

			normals.push_back(normal[0]);
			normals.push_back(normal[1]);
			normals.push_back(normal[2]);

			uvs.push_back(uv[0]);
			uvs.push_back(uv[1]);

			const int index = vertices.size() / 3 - 1;
			indices.push_back(index);
		}
	}
	return ModelMesh(vertices, uvs, normals, indices);
}

ModelMesh OBJLoader::Load(const std::string file_path, const float import_scale)
{
	std::ifstream file(file_path);

	if (!file.good())
	{
		LOG(Error, "OBJLoader cannot locate file '%s'", file_path.c_str());
		return ModelMesh({}, {}, {}, {});
	}

	std::vector<float> raw_vertices;
	std::vector<float> raw_uvs;
	std::vector<float> raw_normals;
	std::vector<int> raw_faces;

	std::string line;

	while (std::getline(file, line)) 
	{
		//Ignore comments and objects (All objects are merged into 1)
		if (line.length() == 0 || line[0] == '#' || line[0] == 'g')
			continue;
		
		std::string identifier = line.substr(0, 2);

		//Vertex
		if (identifier == "v ")
		{
			AppendVertexInfo(line, raw_vertices);
			raw_vertices[raw_vertices.size() - 1] *= import_scale;
			raw_vertices[raw_vertices.size() - 2] *= import_scale;
			raw_vertices[raw_vertices.size() - 3] *= import_scale;
		}

		//UVs
		else if (identifier == "vt")
		{
			AppendVertexInfo(line, raw_uvs);
			raw_uvs.pop_back();//UV has extra 0.0
		}

		//Normals
		else if (identifier == "vn")
			AppendVertexInfo(line, raw_normals);

		//Faces
		else if (identifier == "f ")
			AppendFaceInfo(line, raw_faces);
	}
	file.close();

	return Build(raw_vertices, raw_uvs, raw_normals, raw_faces);
}