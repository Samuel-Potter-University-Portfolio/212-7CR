#include "GameObject.h"
#include "Component.h"
#include "Model.h"
#include "Shader.h"
#include "Game.h"
#include "Logger.h"


GameObject::GameObject()
{
	world = nullptr;
	components_built = false;
}

GameObject::~GameObject()
{
	for (Component* component : components)
		delete component;
}

void GameObject::BuildComponents() 
{
}

void GameObject::SetOwner(World* world)
{
	this->world = world;
}

void GameObject::SetOwner(Object* object)
{
	Super::SetOwner(object);
	GameObject* game_obj = Cast<GameObject>(object);

	if (game_obj)
		this->world = game_obj->world;
	else
		this->world = nullptr;
}

void GameObject::WindowBegin()
{
	Super::WindowBegin();
	components_built = true;
	BuildComponents();
	Begin();
}

void GameObject::LogicTick(float delta_time)
{
	Super::LogicTick(delta_time);
	Tick(delta_time);
	for (Component* component : components)
		component->HandleLogicTick(delta_time);
}

void GameObject::WindowTick(float delta_time)
{
	Super::LogicTick(delta_time);
	for (Component* component : components)
		component->HandleWindowTick(delta_time);
}

void GameObject::AddComponent(Component* component) 
{
	if (components_built)
	{
		LOG(Error, "Cannot add components outside of constructor");
		return;
	}

	component->SetOwner(this);
	components.push_back(component);
}

Model* GameObject::LoadModelAsset(std::string key)
{
	return g_game->GetWindow()->GetModelLoader()[key];
}

Shader* GameObject::LoadShaderAsset(std::string key)
{
	return g_game->GetWindow()->GetShaderLoader()[key];
}

GLuint GameObject::LoadTextureAsset(std::string key)
{
	return g_game->GetWindow()->GetTextureLoader()[key];
}

GLuint GameObject::LoadCubeMapAsset(std::string key)
{
	return g_game->GetWindow()->GetTextureLoader().GetCubeMap(key);
}

void GameObject3D::SetOwner(Object* object)
{
	Super::SetOwner(object);
	SetTransformParent(Cast<Local3D>(object));
}

void GameObject2D::SetOwner(Object* object)
{
	Super::SetOwner(object);
	SetTransformParent(Cast<Local2D>(object));
}

void GameObject3D::WindowBegin()
{
	Super::WindowBegin();
	local_transform.LogicUpdate();
}

void GameObject3D::LogicTick(float delta_time)
{
	local_transform.LogicUpdate();
	Super::LogicTick(delta_time);
}

void GameObject2D::WindowBegin()
{
	Super::WindowBegin();
	local_transform.LogicUpdate();
}

void GameObject2D::LogicTick(float delta_time)
{
	local_transform.LogicUpdate();
	Super::LogicTick(delta_time);
}