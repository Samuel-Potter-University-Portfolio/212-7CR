#pragma once
#include "Component.h"

class CameraComponent : public Component
{
private:
	glm::mat4 projection_matrix;
	float last_aspect_ratio = 0.0f;

public:
	float fov = 90.0f;
	float near_plane = 0.1f;
	float far_plane = 10000.0f;

	inline float GetAspectRatio() { return last_aspect_ratio; }

	void BuildProjectionMatrix(class Window* window);
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix(float lerp);
};

