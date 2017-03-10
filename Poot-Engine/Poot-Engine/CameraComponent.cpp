#include "CameraComponent.h"
#include "Game.h"

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

void CameraComponent::BuildProjectionMatrix(float width, float height)
{
	const float aspect_ratio = width / height;

	if (aspect_ratio == last_aspect_ratio)
		return;

	last_aspect_ratio = aspect_ratio;

	
	if (projection == Perspective || false)
	{
		projection_matrix = glm::perspective(
			RAD_DEG(fov),
			aspect_ratio,
			near_plane,
			far_plane
			);
	}
	else if (projection == Orthographic)
	{
		projection_matrix = glm::ortho(
			-50.0f, //Left
			50.0f,	//Right
			-50.0f, //Bottom
			50.0f,	//Top
			-10.0f, //Back
			100.0f	//Front
		);
	}
}

glm::mat4 CameraComponent::GetProjectionMatrix() 
{
	return projection_matrix;
}