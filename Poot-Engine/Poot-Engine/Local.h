#pragma once
#include "API.h"
#include "Transform.h"
#include "Transform2D.h"


class POOT_API Local3D
{
protected:
	Local3D* transform_parent = nullptr;

public:
	Transform local_transform;

	virtual void SetTransformParent(Local3D* local);

	glm::mat4 GetTransformationMatrix(float lerp_factor);

	glm::vec3 GetWorldLocation();
	glm::vec3 GetWorldLocation(float lerp);

	glm::vec3 GetWorldRotation();
	glm::vec3 GetWorldRotation(float lerp);

	glm::vec3 GetWorldScale();
	glm::vec3 GetWorldScale(float lerp);
};


class POOT_API Local2D
{
protected:
	Local2D* transform_parent = nullptr;

public:
	Transform2D local_transform;

	virtual void SetTransformParent(Local2D* local);

	glm::mat3 GetTransformationMatrix(float lerp_factor);

	glm::vec2 GetWorldLocation();
	glm::vec2 GetWorldLocation(float lerp);

	float GetWorldRotation();
	float GetWorldRotation(float lerp);

	glm::vec2 GetWorldScale();
	glm::vec2 GetWorldScale(float lerp);
};

