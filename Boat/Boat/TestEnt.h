#pragma once
#include "Entity.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "SphereCollider.h"
#include "RigidBody.h"

class TestEnt : public Entity
{
public:
	TestEnt();

	ModelComponent* model_comp;
	CameraComponent* camera_comp;
	SphereCollider* sphere_collider;
	RigidBody* body;

	float track = 90.0f;

protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	virtual void WindowDestroy() override;

};

