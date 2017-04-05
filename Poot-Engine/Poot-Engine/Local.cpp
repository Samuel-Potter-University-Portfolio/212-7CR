#include "Local.h"
#include <gtx\rotate_vector.hpp>
#include <gtx\matrix_transform_2d.hpp>

#define PI 3.141592653589793f
#define RAD_DEG(x) (x * PI/180.0f)

/* Local3D */

void Local3D::SetTransformParent(Local3D* object)
{
	transform_parent = object;
}

glm::mat4 Local3D::GetTransformationMatrix()
{
	if (transform_parent)
		return transform_parent->GetTransformationMatrix() * local_transform.GetMatrix(lerp_time);
	else
		return local_transform.GetMatrix(lerp_time);
}

glm::vec3 Local3D::GetWorldLocation()
{
	if (!transform_parent)
		return local_transform.location;

	const glm::vec3 local_location = local_transform.location;
	const glm::vec3 parent_rotation = transform_parent->local_transform.rotation;

	glm::vec3 location =
		glm::rotateX(
			glm::rotateY(
				glm::rotateZ(local_location, RAD_DEG(parent_rotation.z)),
				RAD_DEG(parent_rotation.y)),
			RAD_DEG(parent_rotation.x));

	return transform_parent->GetWorldLocation() + location;
}

glm::vec3 Local3D::GetWorldLocation(float lerp)
{
	if (!transform_parent)
		return local_transform.GetLerpLocation(lerp);

	const glm::vec3 local_location = local_transform.GetLerpLocation(lerp);
	const glm::vec3 parent_rotation = transform_parent->local_transform.GetLerpRotation(lerp);

	glm::vec3 location =
		glm::rotateX(
			glm::rotateY(
				glm::rotateZ(local_location, RAD_DEG(parent_rotation.z)),
				RAD_DEG(parent_rotation.y)),
			RAD_DEG(parent_rotation.x));

	return transform_parent->GetWorldLocation(lerp) + location;
}

glm::vec3 Local3D::GetWorldRotation()
{
	if (transform_parent)
		return transform_parent->local_transform.rotation + local_transform.rotation;
	else
		return local_transform.rotation;
}

glm::vec3 Local3D::GetWorldRotation(float lerp)
{
	if (transform_parent)
		return transform_parent->local_transform.GetLerpRotation(lerp) + local_transform.GetLerpRotation(lerp);
	else
		return local_transform.GetLerpRotation(lerp);
}

glm::vec3 Local3D::GetWorldScale()
{
	if (transform_parent)
		return transform_parent->local_transform.scale * local_transform.scale;
	else
		return local_transform.scale;
}

glm::vec3 Local3D::GetWorldScale(float lerp)
{
	if (transform_parent)
		return transform_parent->local_transform.GetLerpScale(lerp) * local_transform.GetLerpScale(lerp);
	else
		return local_transform.GetLerpScale(lerp);
}

/* Local2D */

void Local2D::SetTransformParent(Local2D* object)
{
	transform_parent = object;
}

glm::mat3 Local2D::GetTransformationMatrix()
{
	if (transform_parent)
		return transform_parent->GetTransformationMatrix() * local_transform.GetMatrix(lerp_time);
	else
		return local_transform.GetMatrix(lerp_time);
}

glm::vec2 Local2D::GetWorldLocation()
{
	if (!transform_parent)
		return local_transform.location;

	const glm::vec2 local_location = local_transform.location;
	const glm::vec2 location = glm::rotate(local_location, transform_parent->local_transform.rotation);

	return transform_parent->GetWorldLocation() + location;
}

glm::vec2 Local2D::GetWorldLocation(float lerp)
{
	if (!transform_parent)
		return local_transform.GetLerpLocation(lerp);

	const glm::vec2 local_location = local_transform.GetLerpLocation(lerp);
	const glm::vec2 location = glm::rotate(local_location, transform_parent->local_transform.GetLerpRotation(lerp));

	return transform_parent->GetWorldLocation(lerp) + location;
}

float Local2D::GetWorldRotation()
{
	if (transform_parent)
		return transform_parent->local_transform.rotation + local_transform.rotation;
	else
		return local_transform.rotation;
}

float Local2D::GetWorldRotation(float lerp)
{
	if (transform_parent)
		return transform_parent->local_transform.GetLerpRotation(lerp) + local_transform.GetLerpRotation(lerp);
	else
		return local_transform.GetLerpRotation(lerp);
}

glm::vec2 Local2D::GetWorldScale()
{
	if (transform_parent)
		return transform_parent->local_transform.scale * local_transform.scale;
	else
		return local_transform.scale;
}

glm::vec2 Local2D::GetWorldScale(float lerp)
{
	if (transform_parent)
		return transform_parent->local_transform.GetLerpScale(lerp) * local_transform.GetLerpScale(lerp);
	else
		return local_transform.GetLerpScale(lerp);
}