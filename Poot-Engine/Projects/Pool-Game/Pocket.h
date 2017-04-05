#pragma once
#include "GameObject.h"
#include <ModelComponent.h>
#include <SphereCollider.h>


class Pocket : public GameObject3D
{
private:
	ModelComponent* model;
	SphereCollider* sphere;

public:
	Pocket();

	virtual void BuildComponents() override;
};

