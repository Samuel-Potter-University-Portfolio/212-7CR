#include "Mesh.h"



Mesh::Mesh(std::vector<float> vertices, std::vector<int> indices)
	: vertices(vertices), indices(indices)
{
}

Mesh::Mesh(const Mesh& other)
	: vertices(other.vertices), indices(other.indices)
{
}

ModelMesh::ModelMesh(std::vector<float> vertices, std::vector<float> uvs, std::vector<float> normals, std::vector<int> indices) 
	: Mesh(vertices, indices), uvs(uvs), normals(normals)
{

}

ModelMesh::ModelMesh(const ModelMesh& other)
	: Mesh(other), uvs(other.uvs), normals(other.normals)
{
}
