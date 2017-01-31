#pragma once
#include <vector>

/**
Intended to hold raw data about a model.
For use in both collision and rendering

Includes:
-Raw vertices
-Indices to form triangles
*/
class Mesh
{
private:
	std::vector<float> vertices;
	std::vector<int> indices;

public:
	Mesh(std::vector<float> vertices, std::vector<int> indices);
	~Mesh();

	inline std::vector<float> GetRawVertices() const { return vertices; }
	inline std::vector<int> GetRawIndices() const { return indices; }
};

