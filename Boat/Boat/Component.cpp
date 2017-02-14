#include "Component.h"
#include "Entity.h"
#include <gtx\rotate_vector.hpp>

#define PI 3.141592653589793f
#define RAD_DEG(x) (x * PI/180.0f)

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
	if(!parent)
		return transform.location;

	const glm::vec3 local_location = transform.location;
	const glm::vec3 parent_rotation = parent->transform.rotation;

	glm::vec3 location = 
		glm::rotateX(
			glm::rotateY(
				glm::rotateZ(local_location, RAD_DEG(parent_rotation.z)),
			RAD_DEG(parent_rotation.y)),
		RAD_DEG(parent_rotation.x));

	return parent->transform.location + location;
}

glm::vec3 Component::GetWorldLocation(float lerp) 
{
	if (!parent)
		return transform.GetLerpLocation(lerp);

	const glm::vec3 local_location = transform.GetLerpLocation(lerp);
	const glm::vec3 parent_rotation = parent->transform.GetLerpRotation(lerp);

	glm::vec3 location =
		glm::rotateX(
			glm::rotateY(
				glm::rotateZ(local_location, RAD_DEG(parent_rotation.z)),
			RAD_DEG(parent_rotation.y)),
		RAD_DEG(parent_rotation.x));

	return parent->transform.GetLerpLocation(lerp) + location;
}

glm::vec3 Component::GetWorldRotation() 
{
	return parent ? parent->transform.rotation + transform.rotation : transform.rotation;
}

glm::vec3 Component::GetWorldRotation(float lerp) 
{
	return parent ? parent->transform.GetLerpRotation(lerp) + transform.GetLerpRotation(lerp) : transform.GetLerpRotation(lerp);
}

glm::vec3 Component::GetWorldScale()
{
	return parent ? parent->transform.scale + transform.scale : transform.scale;
}

glm::vec3 Component::GetWorldScale(float lerp)
{
	return parent ? parent->transform.GetLerpScale(lerp) + transform.GetLerpScale(lerp) : transform.GetLerpScale(lerp);
}