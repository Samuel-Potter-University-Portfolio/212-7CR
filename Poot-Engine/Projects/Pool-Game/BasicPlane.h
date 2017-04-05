#pragma once
#include <GameObject.h>
#include <ModelComponent.h>

class BasicPlane : public GameObject3D
{
private:
	ModelComponent* model;

public:
	BasicPlane();
	virtual void BuildComponents() override;
};

