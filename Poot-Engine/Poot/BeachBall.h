#pragma once
#include "Entity.h"
#include "ModelComponent.h"
#include "SphereCollider.h"
#include "RigidBody.h"

class BeachBall : public Entity
{
public:
	BeachBall();

	ModelComponent* model_comp;
	SphereCollider* sphere_collider;
	RigidBody* body;


protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	//virtual void LogicTick(float delta_time) override;
	//virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	//virtual void WindowDestroy() override;
};

