#pragma once
#include "API.h"
#include "Component.h"


enum CameraProjection 
{
	Perspective, Orthographic
};

class POOT_API CameraComponent : public Component3D
{
private:
	glm::mat4 projection_matrix;
	float last_aspect_ratio = 0.0f;

public:
	CameraProjection projection = Perspective;
	bool use_latest_location = false;
	bool use_latest_rotation = false;

	float fov = 90.0f;
	float near_plane = 0.1f;
	float far_plane = 10000.0f;

	inline float GetAspectRatio() { return last_aspect_ratio; }

	void BuildProjectionMatrix(float width, float height);
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();
};

