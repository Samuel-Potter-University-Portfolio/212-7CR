#pragma once
#include "Entity.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "FrameBuffer.h"

#define E_TAG_WATER		(E_TAG_LAST * 2)


class WaterSurface : public Entity
{
private:
	ModelComponent* model_comp;
	CameraComponent* camera_comp;
	FrameBuffer frame_buffer;

public:
	WaterSurface();


protected:
	//virtual void LogicBegin() override;
	virtual void WindowBegin() override;

	//virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

	//virtual void LogicDestroy() override;
	virtual void WindowDestroy() override;
};

