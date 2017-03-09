#pragma once
#include <GameObject.h>
#include <ModelComponent.h>

class BasicBox : public GameObject3D
{
private:
	ModelComponent* model;

public:
	BasicBox();

	virtual void BuildComponents() override;
	virtual void Tick(float delta_time) override;
};

