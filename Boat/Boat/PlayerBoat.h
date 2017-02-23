#pragma once
#include "Entity.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "SphereCollider.h"
#include "RigidBody.h"


enum CameraMode 
{
	BoatCam,
	ActionCam
};


class PlayerBoat : public Entity
{
public:
	PlayerBoat();

	CameraMode camera_mode;
	bool last_camera_button_state;

	ModelComponent* model_comp;
	ModelComponent* model_comp_test;
	CameraComponent* camera_comp;
	SphereCollider* sphere_collider;
	RigidBody* body;

	float track = 0.0f;

protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	virtual void WindowDestroy() override;

};

