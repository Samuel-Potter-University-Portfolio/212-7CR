#include "PoolTable.h"
#include "PoolTable.h"
#include <OBJLoader.h>

PoolTable::PoolTable()
{
	SetTags(OBJ_TAG_ENV | POBJ_TAG_TABLE);

	mesh = new Mesh(OBJLoader::Load("Resources/table_surface.obj", 0.2f));

	body_model = MakeComponent<ModelComponent>();
	surface_model = MakeComponent<ModelComponent>();
	//surface_model->local_transform.scale = glm::vec3(1, 3, 1);
	mesh_collider = MakeComponent<MeshCollider>();
	mesh_collider->SetMesh(mesh);
	mesh_collider->local_transform.scale = surface_model->local_transform.scale;
}

PoolTable::~PoolTable() 
{
	delete mesh;
}

void PoolTable::BuildComponents() 
{
	body_model->model = LoadModelAsset("Resources/table_body.obj");
	body_model->shader = LoadShaderAsset("default");
	body_model->SetTextureUnit(0, LoadTextureAsset("Resources/planks.png"));

	surface_model->model = LoadModelAsset("Resources/table_surface.obj");
	surface_model->shader = LoadShaderAsset("default");
	surface_model->SetTextureUnit(0, LoadTextureAsset("Resources/planks.png"));

}
