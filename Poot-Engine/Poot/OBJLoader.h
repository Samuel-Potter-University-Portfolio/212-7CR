#pragma once
#include "API.h"
#include "Mesh.h"
#include <string>


namespace OBJLoader
{
	ModelMesh POOT_API Load(const std::string file_path, const float import_scale);

};


