#include "Component.h"
#include "Entity.h"


glm::mat4 Component::GetTransformationMatrix(float lerp_factor)
{
	return (parent ? parent->transform.GetMatrix(lerp_factor) * transform.GetMatrix(lerp_factor) : transform.GetMatrix(lerp_factor));
}

void Component::LogicTick(float delta_time)
{
	__super::LogicTick(delta_time);
	transform.LogicUpdate();
}

glm::vec3 Component::GetWorldLocation()
{
	return parent ? parent->transform.location + transform.location : transform.location;
}

glm::vec3 Component::GetWorldLocation(float lerp) 
{
	return parent ? parent->transform.GetLerpLocation(lerp) + transform.GetLerpLocation(lerp) : transform.GetLerpLocation(lerp);
}