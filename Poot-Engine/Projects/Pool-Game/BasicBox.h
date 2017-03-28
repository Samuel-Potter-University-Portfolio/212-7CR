#pragma once
#include <GameObject.h>
#include <ModelComponent.h>
#include <SphereCollider.h>

class BasicBox : public GameObject3D
{
private:
	ModelComponent* model;
	SphereCollider* sphere;

public:
	BasicBox();

	virtual void BuildComponents() override;
	virtual void Tick(float delta_time) override;
};

