#pragma once
#include "API.h"

#include <vec3.hpp>
#include <glm.hpp>


class POOT_API Triangle
{
private:
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;

	glm::vec3 normal;
	float plane_d;

public:
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

	inline const glm::vec3& GetNormal() const { return normal; }
	inline const glm::vec3& GetVertA() const { return a; }
	inline const glm::vec3& GetVertB() const { return b; }
	inline const glm::vec3& GetVertC() const { return c; }

	inline float PlaneDistance(const glm::vec3& point) const { return glm::dot(normal, point) + plane_d; }
	inline bool IsFacing(const glm::vec3& point) const { return glm::dot(normal, point) <= 0.0f; }

	bool IsInside(const glm::vec3& point) const;
};

