#pragma once
#include "API.h"
#include <vec3.hpp>
#include <mat4x4.hpp>

enum TransformType
{
	Dynamic, Static
};

class POOT_API Transform
{
private:
	bool matrix_built;
	glm::mat4 matrix;

public:
	Transform(const glm::vec3 location = glm::vec3(0, 0, 0), const glm::vec3 rotation = glm::vec3(0, 0, 0), const glm::vec3 scale = glm::vec3(1, 1, 1));
	TransformType transform_type = Dynamic;

	glm::vec3 previous_location;
	glm::vec3 previous_rotation;
	glm::vec3 previous_scale;

	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;

	void operator=(Transform& other);

	glm::vec3 GetLerpLocation(float lerp);
	glm::vec3 GetLerpRotation(float lerp);
	glm::vec3 GetLerpScale(float lerp);

	void LogicUpdate();
	inline void ForceUpdateLocation() { previous_location = location; }
	inline void ForceUpdateRotation() { previous_rotation = rotation; }
	inline void ForceUpdateScale() { previous_scale = scale; }

	glm::mat4& GetMatrix(float lerp_factor);

	Transform operator+(Transform& other);

	glm::vec3 GetForward();
	glm::vec3 GetXZForward();

	glm::vec3 GetRight();
	glm::vec3 GetXZRight();

	glm::vec3 GetUp();
};

