#include "Mesh.h"



Mesh::Mesh(std::vector<float> vertices, std::vector<int> indices)
	: vertices(vertices), indices(indices)
{
}

ModelMesh::ModelMesh(std::vector<float> vertices, std::vector<float> uvs, std::vector<float> normals, std::vector<int> indices) 
	: Mesh(vertices, indices), uvs(uvs), normals(normals)
{

}
