#pragma once
#include <GameObject.h>
#include <InputComponent.h>
#include <CameraComponent.h>
#include <Body.h>
#include <vector>

#include "PoolBall.h"


class PoolPlayer : public GameObject3D
{
private:
	CameraComponent* camera;
	InputComponent* input_component;

	PoolBall* cue_ball = nullptr;
	Body* cue_ball_body = nullptr;
	std::vector<PoolBall*> pool_balls;

	enum Mode 
	{
		Shooting,
		Watching
	} currrent_mode = Shooting;

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

	void Shoot(float power);
	void FinishWatching();
	bool AreBallsStill();
};

