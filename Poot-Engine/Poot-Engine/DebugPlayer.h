#pragma once
#include "API.h"
#include "GameObject.h"

class POOT_API DebugPlayer : public GameObject3D
{
public:
	DebugPlayer();

protected:
	//virtual void BuildComponents() override;

	virtual void Tick(float delta_time) override;
};

