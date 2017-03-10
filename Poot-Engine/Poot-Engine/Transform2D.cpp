#include "Transform2D.h"

#include <math.h>
#include <glm.hpp>
#include <common.hpp>
#include <gtx\matrix_transform_2d.hpp>


#define PI 3.141592653589793f
#define RAD_DEG(x) (x * PI/180.0f)


Transform2D::Transform2D(const glm::vec2 location, const glm::vec2 scale, const float rotation)
	: location(location), rotation(rotation), scale(scale),
	previous_location(location), previous_rotation(rotation), previous_scale(scale)
{

}


void Transform2D::operator=(Transform2D& other)
{
	location = other.location;
	rotation = other.rotation;
	scale = other.scale;

	previous_location = other.previous_location;
	previous_rotation = other.previous_rotation;
	previous_scale = other.previous_scale;

	transform_type = other.transform_type;

	if (other.matrix_built)
	{
		matrix = other.matrix;
		matrix_built = true;
	}
}

#define CLAMP_ROT(x) { \
	while (x < -180.0f) \
		x += 360.0f; \
	while (x > 180.0f) \
		x -= 360.0f; \
} 

void Transform2D::LogicUpdate()
{
	if (matrix_built && transform_type == Static)
		return;
	
	CLAMP_ROT(rotation);

	previous_location = location;
	previous_rotation = rotation;
	previous_scale = scale;
}

glm::vec2 Transform2D::GetLerpLocation(float lerp)
{
	return previous_location * lerp + location * (1.0f - lerp);
}

float Transform2D::GetLerpRotation(float lerp)
{
	return tan(atan(previous_rotation) * lerp + atan(rotation) * (1.0f - lerp));
}

glm::vec2 Transform2D::GetLerpScale(float lerp)
{
	return previous_scale * lerp + scale * (1.0f - lerp);
}

glm::mat3& Transform2D::GetMatrix(float lerp_factor)
{
	if (matrix_built && transform_type == Static)
		return matrix;

	matrix = glm::mat3(1.0);

	glm::translate(matrix, glm::vec2(0, 0));

	matrix = glm::translate(matrix, GetLerpLocation(lerp_factor));
	matrix = glm::rotate(matrix, RAD_DEG(GetLerpRotation(lerp_factor)));
	matrix = glm::scale(matrix, GetLerpScale(lerp_factor));
	matrix_built = true;

	return matrix;
}

Transform2D Transform2D::operator+(Transform2D& other)
{
	Transform2D trans
		(
			location + other.location,
			scale * other.scale,
			rotation + other.rotation
			);

	trans.previous_location = previous_location + other.previous_location;
	trans.previous_rotation = previous_rotation + other.rotation;
	trans.previous_scale = previous_scale * other.previous_scale;

	return trans;
}