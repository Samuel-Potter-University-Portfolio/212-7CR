#pragma once
#include "API.h"
#include "Entity.h"

class POOT_API DebugPlayer : public Entity
{
public:
	DebugPlayer();

	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;
};

