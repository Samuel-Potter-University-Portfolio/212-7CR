#include "Component.h"
#include "GameObject.h"


Component::Component()
{
	SetTags(OBJ_TAG_COMPONENT);
}


Component::~Component()
{
}

void Component::WindowBegin() 
{
	Super::WindowBegin();
	Begin();
}

void Component::LogicTick(float delta_time) 
{
	Tick(delta_time);
	Super::LogicTick(delta_time);
}

void Component3D::LogicTick(float delta_time)
{
	Super::LogicTick(delta_time);
	local_transform.LogicUpdate();
}

void Component2D::LogicTick(float delta_time)
{
	Super::LogicTick(delta_time);
	local_transform.LogicUpdate();
}
	
void Component3D::SetOwner(Object* object) 
{
	Super::SetOwner(object);

	GameObject3D* game_object = Cast<GameObject3D>(object);

	if(game_object)
		SetTransformParent(game_object);
}

void Component2D::SetOwner(Object* object)
{
	Super::SetOwner(object);

	GameObject2D* game_object = Cast<GameObject2D>(object);

	if (game_object)
		SetTransformParent(game_object);
}