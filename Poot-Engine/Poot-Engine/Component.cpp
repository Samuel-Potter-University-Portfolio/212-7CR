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
	Super::LogicTick(delta_time);

	if (IsEnabled())
		Tick(delta_time);
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

void Component3D::WindowBegin()
{
	Super::WindowBegin();
	local_transform.LogicUpdate();
}

void Component3D::LogicTick(float delta_time)
{
	local_transform.LogicUpdate();
	lerp_time = 0.0f;
	frame_timestep = delta_time != 0.0f ? delta_time : 1.0f;

	Super::LogicTick(delta_time);
}

void Component3D::WindowTick(float delta_time)
{
	Super::WindowTick(delta_time);
	lerp_time += delta_time / frame_timestep;
	if (lerp_time > 1.0f)
		lerp_time = 1.0f;
}

void Component2D::WindowBegin()
{
	Super::WindowBegin();
	local_transform.LogicUpdate();
}

void Component2D::LogicTick(float delta_time)
{
	local_transform.LogicUpdate();
	lerp_time = 0.0f;
	frame_timestep = delta_time != 0.0f ? delta_time : 1.0f;

	Super::LogicTick(delta_time);
}

void Component2D::WindowTick(float delta_time)
{
	Super::WindowTick(delta_time);
	lerp_time += delta_time / frame_timestep;
	if (lerp_time > 1.0f)
		lerp_time = 1.0f;
}