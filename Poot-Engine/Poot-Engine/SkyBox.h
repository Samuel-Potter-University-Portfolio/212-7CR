#pragma once
#include "API.h"
#include "GameObject.h"
#include "ModelComponent.h"


class POOT_API SkyBox : public GameObject3D
{
private:
	ModelComponent* model_comp;

public:
	SkyBox();

protected:
	virtual void BuildComponents() override;
};

