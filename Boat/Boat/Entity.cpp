#include "Entity.h"
#include "Logger.h"

Entity::~Entity() 
{
	for (Component* component : components)
		delete component;
}

void Entity::LogicDestroy() 
{
	for (Component* component : components)
		component->LogicDestroy();
}

void Entity::WindowDestroy() 
{
	for (Component* component : components)
		component->WindowDestroy();
}

void Entity::AddComponent(Component* component) 
{
	//Only allow to add components, if entity has yet to begin
	if (!HasBegun())
	{
		components.push_back(component);
		component->SetParent(this);
	}
	else
		LOG(Error, "Cannot add component as entity is already ready");
}

void Entity::LogicBegin() 
{
	__super::LogicBegin();

	//Begin components
	for (Component* component : components)
		if (component->IsActive())
			component->ForceLogicBegin();
}

void Entity::WindowBegin() 
{
	__super::WindowBegin();

	//Begin components
	for (Component* component : components)
		if (component->IsActive())
			component->ForceWindowBegin();
}

void Entity::LogicTick(float delta_time) 
{
	__super::LogicTick(delta_time);
	transform.LogicUpdate();

	//Tick components
	for (Component* component : components)
		if(component->IsActive())
			component->HandleLogicTick(delta_time);

}

void Entity::WindowTick(float delta_time) 
{
	__super::WindowTick(delta_time);

	//Tick components
	for (Component* component : components)
		if (component->IsActive())
			component->HandleWindowTick(delta_time);
}