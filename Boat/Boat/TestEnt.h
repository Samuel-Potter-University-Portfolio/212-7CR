#pragma once
#include "Entity.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "SphereColliderComponent.h"
#include "RigidBody.h"

class TestEnt : public Entity
{
public:
	TestEnt();

	ModelComponent* model_comp;
	CameraComponent* camera_comp;
	SphereColliderComponent* sphere_collider;
	RigidBody* body;

protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	virtual void WindowDestroy() override;

};

