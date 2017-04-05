#pragma once
#include "API.h"
#include "Transform.h"
#include <vec2.hpp>
#include <mat3x3.hpp>

class POOT_API Transform2D
{
private:
	bool matrix_built;
	glm::mat3 matrix;

public:
	Transform2D(const glm::vec2 location = glm::vec2(0, 0), const glm::vec2 scale = glm::vec2(1, 1), const float rotation = 0.0f);
	TransformType transform_type = Dynamic;

	glm::vec2 previous_location;
	glm::vec2 previous_scale;
	float previous_rotation;

	glm::vec2 location;
	glm::vec2 scale;
	float rotation;

	void operator=(Transform2D& other);

	glm::vec2 GetLerpLocation(float lerp);
	glm::vec2 GetLerpScale(float lerp);
	float GetLerpRotation(float lerp);

	void LogicUpdate();
	inline void ForceUpdateLocation() { previous_location = location; }
	inline void ForceUpdateRotation() { previous_rotation = rotation; }
	inline void ForceUpdateScale() { previous_scale = scale; }

	glm::mat3& GetMatrix(float lerp_factor);

	Transform2D operator+(Transform2D& other);
};

