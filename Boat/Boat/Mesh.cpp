#include "Mesh.h"



Mesh::Mesh(std::vector<float> vertices, std::vector<int> indices)
	: vertices(vertices), indices(indices)
{
}


Mesh::~Mesh()
{
}
