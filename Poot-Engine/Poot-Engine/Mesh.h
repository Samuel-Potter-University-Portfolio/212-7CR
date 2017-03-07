#pragma once
#include "API.h"
#include <vector>

/**
Intended to hold raw data about a model.
For use in both collision and rendering

Includes:
-Raw vertices
-Indices to form triangles
*/
class POOT_API Mesh
{
private:
	std::vector<float> vertices;
	std::vector<int> indices;

public:
	Mesh(std::vector<float> vertices, std::vector<int> indices);

	inline std::vector<float> GetRawVertices() const { return vertices; }
	inline std::vector<int> GetRawIndices() const { return indices; }
};

/**
Intended to hold additional data for a model
for use in rendering

Includes:
-Raw vertices
-Indices to form triangles
-UVs
-Normals
*/
class POOT_API ModelMesh : public Mesh
{
private:
	std::vector<float> uvs;
	std::vector<float> normals;

public:
	ModelMesh(std::vector<float> vertices, std::vector<float> uvs, std::vector<float> normals, std::vector<int> indices);

	inline std::vector<float> GetRawUVs() const { return uvs; }
	inline std::vector<float> GetRawNormals() const { return normals; }

};