#pragma once
#include <GameObject.h>
#include <InputComponent.h>
#include <CameraComponent.h>
#include <ModelComponent.h>
#include <Body.h>
#include <vector>

#include <Canvas.h>
#include <TextElement.h>

#include "PoolBall.h"


class PoolPlayer : public GameObject3D
{
private:
	CameraComponent* camera;
	InputComponent* input_component;
	ModelComponent* model;

	Canvas* main_canvas;
	TextElement* force_text;

	PoolBall* cue_ball = nullptr;
	Body* cue_ball_body = nullptr;
	std::vector<PoolBall*> pool_balls;

	enum Mode 
	{
		Shooting,
		Watching
	} currrent_mode = Shooting;
	bool is_shooting = false;
	float shot_power = 0.0f;
	float shot_timer = 0.0f;

public:
	PoolPlayer();
	~PoolPlayer();

protected:
	virtual void BuildComponents() override;

	void OnLookSideways(float amount);

	void OnHit(bool pressed);

	void OnGrabMouse(bool pressed);
	void OnReleaseMouse(bool pressed);

	virtual void Tick(float delta_time) override;

	void Shoot();
	void FinishWatching();
	bool AreBallsStill();
};

