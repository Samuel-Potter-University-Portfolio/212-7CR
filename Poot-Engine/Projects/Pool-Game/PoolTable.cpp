#include "PoolTable.h"
#include "PoolWorld.h"
#include <OBJLoader.h>
#include <DefaultShader.h>

PoolTable::PoolTable()
{
	SetTags(OBJ_TAG_ENV | POBJ_TAG_TABLE);

	mesh = new Mesh(OBJLoader::Load("Resources/table_surface.obj", 0.2f));

	body_model = MakeComponent<ModelComponent>();
	surface_model = MakeComponent<ModelComponent>();
	//surface_model->local_transform.scale = glm::vec3(1, 3, 1);
	mesh_collider = MakeComponent<MeshCollider>();
	mesh_collider->SetMesh(mesh);
	mesh_collider->properties.friction = 0.03f;
	mesh_collider->properties.restitution = 1.0f;
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

	surface_model->SetFloatUnit(SHADER_UNITF_ROUGHNESS, 0.5f);
	surface_model->SetFloatUnit(SHADER_UNITF_SHININESS, 50.0f);
	surface_model->SetFloatUnit(SHADER_UNITF_REFLECTIVENESS, 0.35f);

	surface_model->SetIntUnit(SHADER_UNITI_USING_PHONG_MAP, 1);
	surface_model->SetTextureUnit(SHADER_UNITT_BASE_TEXTURE, LoadTextureAsset("Resources/Table_UV_Map.png"));
	surface_model->SetTextureUnit(SHADER_UNITT_PHONG_MAP, LoadTextureAsset("Resources/Table_Phong_Map.png"));
	surface_model->SetTextureUnit(SHADER_UNITT_REFLECTION_CM, LoadCubeMapAsset("Resources/Skybox/TropicalSunnyDay.png"));
}
