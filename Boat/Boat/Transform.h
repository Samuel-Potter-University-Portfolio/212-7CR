#pragma once
#include <vec3.hpp>
#include <mat4x4.hpp>

enum TransformType
{
	Dynamic, Static
};

class Transform
{
private:
	bool matrix_built;
	glm::mat4 matrix;

	glm::vec3 previous_location;
	glm::vec3 previous_rotation;
	glm::vec3 previous_scale;

protected:
	TransformType transform_type;

	inline glm::vec3 GetLerpLocation(float lerp);
	inline glm::vec3 GetLerpRotation(float lerp);
	inline glm::vec3 GetLerpScale(float lerp);

public:
	Transform(const glm::vec3 location = glm::vec3(0, 0, 0), const glm::vec3 rotation = glm::vec3(0, 0, 0), const glm::vec3 scale = glm::vec3(1, 1, 1));

	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;

	void LogicUpdate();
	glm::mat4& GetMatrix(float lerp_factor);

	Transform operator+(Transform& other);
};
