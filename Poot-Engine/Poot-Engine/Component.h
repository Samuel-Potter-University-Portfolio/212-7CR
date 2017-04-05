#pragma once
#include "API.h"
#include "Tickable.h"
#include "Object.h"
#include "Local.h"

/*
Basic component meant for attaching to game objects
Every logic process should be called through the component's parent,
including memory management
*/
class POOT_API Component : public Object
{
public:
	Component();
	virtual ~Component();

protected:
	//virtual void LogicBegin() {}
	virtual void WindowBegin() override;

	virtual void LogicTick(float delta_time) override;
	//virtual void WindowTick(float delta_time) {}

	virtual void Begin() {}
	virtual void Tick(float delta_time) {}
};

class POOT_API Component3D : public Local3D, public Component
{
protected:
	float frame_timestep = 1.0f;

	virtual void WindowBegin() override;
	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

public:
	virtual void SetOwner(Object* object) override;
};

class POOT_API Component2D : public Local2D, public Component
{
protected:
	float frame_timestep = 1.0f;

	virtual void WindowBegin() override;
	virtual void LogicTick(float delta_time) override;
	virtual void WindowTick(float delta_time) override;

public:
	virtual void SetOwner(Object* object) override;
};

