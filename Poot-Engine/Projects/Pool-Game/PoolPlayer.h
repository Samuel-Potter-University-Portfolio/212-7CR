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


#define CONSOLE_BUFFER_SIZE  10

class PoolPlayer : public GameObject3D
{
private:
	CameraComponent* camera;
	InputComponent* input_component;
	ModelComponent* model;

	Canvas* main_canvas;

	TextElement* force_text;

	std::string console_buffer[CONSOLE_BUFFER_SIZE];
	TextElement* console_text;

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

	bool shot_in_progress = false;

public:
	PoolPlayer();
	~PoolPlayer();

protected:
	virtual void BuildComponents() override;

	void OnLookSideways(float amount);

	void OnHit(bool pressed);
	void OnReset(bool pressed);
	void OnChangeCameraMode(bool pressed);

	void OnGrabMouse(bool pressed);
	void OnReleaseMouse(bool pressed);

	virtual void Tick(float delta_time) override;

	void Print(std::string message);

	void Shoot();
	void FinishWatching();
	bool AreBallsStill();

public:
	void OnPotted(PoolBall* ball);
	void OnFloored(PoolBall* ball);
};

