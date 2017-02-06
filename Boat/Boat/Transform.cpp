#include "Transform.h"

#include <math.h>
#include <glm.hpp>
#include <common.hpp>
#include <mat4x4.hpp>
#include <gtx\transform.hpp>


#define PI 3.141592653589793f
#define RAD_DEG(x) (x * PI/180.0f)


Transform::Transform(const glm::vec3 location, const glm::vec3 rotation, const glm::vec3 scale)
	: location(location), rotation(rotation), scale(scale),
	  previous_location(location), previous_rotation(rotation), previous_scale(scale)
{

}

void Transform::LogicUpdate() 
{
	if (matrix_built && transform_type == Static)
		return;

	previous_location = location;
	previous_rotation = rotation;
	previous_scale = scale;
}

inline float LerpAngle(float a, float b, float lerp)
{
	return tan(atan(a) * lerp + atan(b) * (1.0f - lerp));
}

glm::vec3 Transform::GetLerpLocation(float lerp) 
{
	return previous_location * lerp + location * (1.0f - lerp);
}

glm::vec3 Transform::GetLerpRotation(float lerp)
{
	return
		glm::vec3(
			LerpAngle(previous_rotation.x, rotation.x, lerp),
			LerpAngle(previous_rotation.y, rotation.y, lerp),
			LerpAngle(previous_rotation.z, rotation.z, lerp)
		);
}

glm::vec3 Transform::GetLerpScale(float lerp)
{
	return previous_scale * lerp + scale * (1.0f - lerp);
}

glm::mat4& Transform::GetMatrix(float lerp_factor) 
{
	if (matrix_built && transform_type == Static)
		return matrix;

	matrix = glm::mat4(1.0);

	matrix = glm::translate(matrix, GetLerpLocation(lerp_factor));
	matrix = glm::scale(matrix, GetLerpScale(lerp_factor));

	glm::vec3 rotation = GetLerpRotation(lerp_factor);
	matrix = glm::rotate(matrix, RAD_DEG(rotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, RAD_DEG(rotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, RAD_DEG(rotation.z), glm::vec3(0, 0, 1));
	matrix_built = true;

	return matrix;
}

Transform Transform::operator+(Transform& other) 
{
	Transform trans
	(
		location + other.location,
		rotation + other.rotation,
		scale * other.scale
	);

	trans.previous_location = previous_location + other.previous_location;
	trans.previous_rotation = previous_rotation + other.rotation;
	trans.previous_scale = previous_scale * other.previous_scale;

	return trans;
}

glm::vec3 Transform::GetForward()
{
	return glm::normalize(glm::vec3(
		sin(RAD_DEG(rotation.y)),
		tan(RAD_DEG(rotation.x)),
		cos(RAD_DEG(rotation.y))
		));
}

glm::vec3 Transform::GetXZForward()
{
	return glm::normalize(glm::vec3(
		sin(RAD_DEG(rotation.y)),
		0,
		cos(RAD_DEG(rotation.y))
		));
}

glm::vec3 Transform::GetRight() 
{
	return glm::normalize(
		glm::cross(GetForward(), glm::vec3(0, 1, 0))
		);
}

glm::vec3 Transform::GetXZRight() 
{
	glm::vec3 right = GetRight();
	right.y = 0;
	return glm::normalize(right);
}

glm::vec3 Transform::GetUp() 
{
	return glm::normalize(
		glm::cross(GetForward(), -GetRight())
		);
}