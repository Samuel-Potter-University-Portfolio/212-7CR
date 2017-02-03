#pragma once
#include "Mesh.h"
#include <string>


namespace OBJLoader
{
	ModelMesh Load(const std::string file_path, const float import_scale);

};


