#include "Component.h"
#include "Entity.h"


Transform Component::GetWorldTransform() 
{
	return parent ? parent->transform + transform : transform;
}

void Component::LogicTick(float delta_time)
{
	__super::LogicTick(delta_time);
	transform.LogicUpdate();
}