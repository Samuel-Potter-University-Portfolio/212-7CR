#pragma once
#include <GameObject.h>
#include <InputComponent.h>
#include <CameraComponent.h>
#include <Canvas.h>


class PoolEntry : public GameObject3D
{
private:
	InputComponent* input_component;
	Canvas* main_canvas;
	CameraComponent* camera;

public:
	PoolEntry();

protected:
	virtual void BuildComponents() override;
	virtual void Tick(float delta_time) override;

	void OnEnter(bool pressed);
};

