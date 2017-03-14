#pragma once
#include "API.h"
#include <GameObject.h>
#include <InputComponent.h>
#include <ModelComponent.h>
#include <Body.h>


class PoolDebugPlayer : public GameObject3D
{
private:
	ModelComponent* model_component;
	InputComponent* input_component;
	Body* body;
	glm::vec3 current_input;
	float upward_input = 0.0f;

public:
	PoolDebugPlayer();

protected:
	void OnForward(float amount);
	void OnStrafe(float amount);

	void OnLookUp(float amount);
	void OnLookSideways(float amount);

	void OnJump(bool pressed);
	void OnCrouch(bool pressed);

	void OnGrabMouse(bool pressed);
	void OnReleaseMouse(bool pressed);

	virtual void BuildComponents() override;

	virtual void Tick(float delta_time) override;
};
