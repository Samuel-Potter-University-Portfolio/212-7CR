#pragma once
#include "Entity.h"
#include "ModelComponent.h"


class SkyBox : public Entity
{
private:
	ModelComponent* model_comp;

public:
	SkyBox();

protected:
	virtual void WindowBegin() override;
};

