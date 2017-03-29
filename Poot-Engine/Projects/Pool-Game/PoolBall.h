#pragma once
#include <GameObject.h>
#include <ModelComponent.h>
#include <SphereCollider.h>

class PoolBall : public GameObject3D
{
private:
	ModelComponent* model;
	SphereCollider* sphere;
	int number;

public:
	PoolBall(int number);

	virtual void BuildComponents() override;
	virtual void Tick(float delta_time) override;
};

