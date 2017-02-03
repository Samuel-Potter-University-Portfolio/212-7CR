#pragma once
#include "Entity.h"
#include "ModelComponent.h"
#include "CameraComponent.h"

class TestEnt : public Entity
{
public:
	TestEnt();

	ModelComponent* model_comp;
	ModelComponent* model_comp2;
	ModelComponent* model_comp3;
	CameraComponent* camera_comp;

protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	virtual void WindowDestroy() override;

};

