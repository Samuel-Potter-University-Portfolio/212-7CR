#include "CameraComponent.h"
#include "Game.h"
#include "Logger.h"

#include <math.h>
#include <glm.hpp>
#include <common.hpp>
#include <mat4x4.hpp>
#include <gtx\transform.hpp>


#define PI 3.141592653589793f
#define RAD_DEG(x) (x * PI/180.0f)


glm::mat4 CameraComponent::GetViewMatrix()
{
	const glm::vec3 rotation = use_latest_rotation ? GetWorldRotation() : GetWorldRotation(lerp_time);
	const glm::vec3 eyes = use_latest_location ? GetWorldLocation() : GetWorldLocation(lerp_time);

	const glm::vec3 look_at = eyes + glm::vec3(
		sin(RAD_DEG(rotation.y)),
		tan(RAD_DEG(rotation.x)),
		cos(RAD_DEG(rotation.y))
	);

	return glm::lookAt(
		eyes,
		look_at,
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
}

void CameraComponent::BuildProjectionMatrix(float aspect_ratio)
{
	if (aspect_ratio == last_aspect_ratio)
		return;

	last_aspect_ratio = aspect_ratio;

	projection_matrix = glm::perspective(
		RAD_DEG(fov),
		last_aspect_ratio,
		near_plane,
		far_plane
	);

	LOG(Log, "Built view matrix {fov:%i, aspect:%f, near:%i, far:%i}", (int)fov, last_aspect_ratio, (int)near_plane, (int)far_plane);
}

glm::mat4 CameraComponent::GetProjectionMatrix() 
{
	return projection_matrix;
}