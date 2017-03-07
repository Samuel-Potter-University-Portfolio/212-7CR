#pragma once
#include "API.h"
#include "Entity.h"
#include "ModelComponent.h"


class POOT_API SkyBox : public Entity
{
private:
	ModelComponent* model_comp;

public:
	SkyBox();

protected:
	virtual void WindowBegin() override;
};

