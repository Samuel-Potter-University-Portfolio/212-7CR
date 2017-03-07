#pragma once
#include "Entity.h"
#include "ModelComponent.h"
#include "CameraComponent.h"

class PlayerEntry : public Entity
{
public:
	ModelComponent* model_comp;
	CameraComponent* camera_comp;

	PlayerEntry();

protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	//virtual void WindowDestroy() override;
};

